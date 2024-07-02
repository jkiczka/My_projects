import argparse
import pandas as pd
import numpy as np
from sklearn.decomposition import NMF, TruncatedSVD
from sklearn.metrics import mean_squared_error
import torch
import random
import warnings

def make_matrix(df, num_users, num_movies):
    Z = np.full((num_users, num_movies), -1, dtype=np.float64)
    for index, row in df.iterrows():
        uid = int(row['userId'])
        mid = int(row['movieId2'])
        rating = row['rating']
        Z[uid-1, mid-1] = rating
    return Z

def replace_minus_one_with_weighted_mean(T, alpha):
    Z = T.astype(float)  
    Z[Z == -1] = np.nan  
    
    with warnings.catch_warnings():
        warnings.simplefilter("ignore", category=RuntimeWarning)
        column_means = np.nanmean(Z, axis=0)
        row_means = np.nanmean(Z, axis=1)

    row_means[np.isnan(row_means)] = 3.5
    column_means[np.isnan(column_means)] = 3.5

    for i in range(Z.shape[0]):
        for j in range(Z.shape[1]):
            if np.isnan(Z[i, j]):
                Z[i, j] = (1 - alpha) * column_means[j] + alpha * row_means[i]

    return Z

def compute_rmse(actual, approximated, rows, cols):
    actual_values = actual[rows, cols]
    approximated_values = approximated[rows, cols]
    squared_errors = (actual_values - approximated_values) ** 2
    mean_squared_error = np.mean(squared_errors)
    rmse = np.sqrt(mean_squared_error)
    return rmse

def NMF_RMSE(n_comp, train_m, original_m, rows_indices, cols_indices):
    model = NMF(n_components=n_comp, init='random', random_state=0)
    W = model.fit_transform(train_m)
    H = model.components_
    Z_approximated = W.dot(H)
    return compute_rmse(original_m, Z_approximated, rows_indices, cols_indices)

def SVD1_RMSE(n_comp, train_m, org_m, rows_indices, cols_indices):
    svd = TruncatedSVD(n_components=n_comp, random_state=42)
    svd.fit(train_m)
    Sigma2 = np.diag(svd.singular_values_)
    VT = svd.components_
    W = svd.transform(train_m) / svd.singular_values_
    H = np.dot(Sigma2, VT)
    Z_approximated = np.dot(W, H)
    return compute_rmse(org_m, Z_approximated, rows_indices, cols_indices)

def iterated_SVD(n_comp, Z, train_rows_indices, train_cols_indices, num_iter):
    for i in range(num_iter):
        svd = TruncatedSVD(n_components=n_comp, random_state=42)
        svd.fit(Z)
        Sigma2 = np.diag(svd.singular_values_)
        VT = svd.components_
        W = svd.transform(Z) / svd.singular_values_
        H = np.dot(Sigma2, VT)
        Z_approximated = np.dot(W, H)
        Z_approximated[train_rows_indices, train_cols_indices] = Z[train_rows_indices, train_cols_indices]
        Z = Z_approximated
    return Z

def SVD2_RMSE(n_comp, Z, org_Z, train_row, train_col, cols_indices, rows_indices, num_iter):
    Z_approximated = iterated_SVD(n_comp, Z, train_row, train_col, num_iter)
    return compute_rmse(org_Z, Z_approximated, rows_indices, cols_indices)

def update_matrices(W, H, z_ij, lr, lmbda, i, j):
    w_i=W[i,:]
    h_j=H[:, j]

    delta_w=-2*lr*((z_ij - torch.dot(w_i, h_j)) * h_j + lmbda * w_i)
    delta_h =-2*lr*((z_ij - torch.dot(w_i, h_j)) * w_i + lmbda * h_j)

    W[i, :]-=delta_w
    H[:, j]-=delta_h

def SGD(X, num_factors, num_iterations, lr, lmbda, row_idx, col_idx):
    num_rows, num_cols = X.shape
    mean = np.mean(X)
    W = torch.full((num_rows, num_factors), np.sqrt(mean)/np.sqrt(num_factors))
    H = torch.full((num_factors, num_cols), np.sqrt(mean)/np.sqrt(num_factors))

    prev_norm = float('inf')  
    for iteration in range(num_iterations):
        idx = random.randint(0, len(col_idx) - 1)
        i = row_idx[idx]
        j = col_idx[idx]
        z_ij = X[i][j]
        update_matrices(W, H, z_ij, lr, lmbda, i, j)

        current_norm = torch.norm(W) + torch.norm(H)
        
        if abs(current_norm - prev_norm) < 10e-47:
            print(f"Converged at iteration {iteration}")
            break

        prev_norm = current_norm

    return np.dot(W.detach().numpy(), H.detach().numpy())

def SGD_RMSE(X, org_Z, num_factors, num_iterations, lr, lmbda, train_row, train_col, cols_indices, rows_indices):
    Z_approx=SGD(X, num_factors, num_iterations, lr, lmbda, train_row, train_col)
    return compute_rmse(org_Z, Z_approx, rows_indices, cols_indices)

def work_with_files(train_file, test_file):
    train_df = pd.read_csv(train_file, names=['userId', 'movieId', 'rating', 'timestamp'], skiprows=1)
    train_df = train_df.sort_values(by=['userId', 'movieId'])
    test_df = pd.read_csv(test_file, names=['userId', 'movieId', 'rating', 'timestamp'], skiprows=1)
    test_df = test_df.sort_values(by=['userId', 'movieId'])
    all_df = pd.concat([test_df, train_df])

    id_mapping = {}
    consecutive_id = 1

    for id_value in all_df['movieId']:
        if id_value not in id_mapping:
            id_mapping[id_value] = consecutive_id
            consecutive_id += 1

    all_df['movieId2'] = all_df['movieId'].map(id_mapping)

    test_df = pd.merge(test_df, all_df, on=['userId', 'movieId', 'rating'], how='inner')
    train_df = pd.merge(train_df, all_df, on=['userId', 'movieId', 'rating'], how='inner')

    num_users = len(all_df['userId'].unique())
    num_movies = max(all_df['movieId2'].unique())

    Z = make_matrix(all_df, num_users, num_movies)

    train_matrix = make_matrix(train_df, num_users, num_movies)
    test_matrix = make_matrix(test_df, num_users, num_movies)
    train_row_indices, train_col_indices = np.where(train_matrix != -1)
    test_row_indices, test_col_indices = np.where(test_matrix != -1)

    return Z, train_matrix, test_matrix, train_row_indices, train_col_indices, test_row_indices, test_col_indices

def main(train_file, test_file, alg, result_file):
    Z, train_matrix, test_matrix, train_row_indices, train_col_indices, test_row_indices, test_col_indices = work_with_files(train_file, test_file)
    train_matrix=replace_minus_one_with_weighted_mean(train_matrix, 0.59)
    if alg == 'NMF':
        rmse = NMF_RMSE(n_comp=36, train_m=train_matrix, original_m=Z, rows_indices=test_row_indices, cols_indices=test_col_indices)
    elif alg == 'SVD1':
        rmse = SVD1_RMSE(n_comp=7, train_m=train_matrix, org_m=Z, rows_indices=test_row_indices, cols_indices=test_col_indices)
    elif alg == 'SVD2':
        rmse = SVD2_RMSE(n_comp=2, Z=train_matrix, org_Z=Z, train_row=train_row_indices, train_col=train_col_indices, cols_indices=test_col_indices, rows_indices=test_row_indices, num_iter=33)
    elif alg == 'SGD':
        rmse = SGD_RMSE(X=train_matrix, org_Z=Z, num_factors=4, num_iterations=6000000, lr=0.0005, lmbda=0, train_row=train_row_indices, train_col=train_col_indices, cols_indices=test_col_indices, rows_indices=test_row_indices)
    else:
        raise ValueError("Invalid algorithm. Please choose one of: NMF, SVD1, SVD2, SGD")

    with open(result_file, 'w') as f:
        f.write(str(rmse))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Recommendation System')
    parser.add_argument('--train', type=str, help='File with training data')
    parser.add_argument('--test', type=str, help='File with test data')
    parser.add_argument('--alg', type=str, help='Algorithm: NMF, SVD1, SVD2, SGD')
    parser.add_argument('--result', type=str, help='File to save the result')
    args = parser.parse_args()

    main(args.train, args.test, args.alg.upper(), args.result)

