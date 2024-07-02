import argparse
import pandas as pd
from hmmlearn.hmm import GaussianHMM
import numpy as np
import os


def classify_devices(train_file, test_folder, output_file):
    data = pd.read_csv(train_file)
    l2 = data['lighting2'].values
    l5 = data['lighting5'].values
    l4 = data['lighting4'].values
    r = data['refrigerator'].values
    m = data['microwave'].values
    devs=['lighting2', 'lighting5', 'lighting4','microwave','refrigerator']
    model1 = GaussianHMM(n_components=17, random_state=22, n_iter=300)
    seq=[[i] for i in l2]
    model1.fit(seq)
    model2 = GaussianHMM(n_components=12, random_state=28, n_iter=300)
    seq=[[i] for i in l5]
    model2.fit(seq)
    model3 = GaussianHMM(n_components=15, random_state=31, n_iter=300)
    seq=[[i] for i in l4]
    model3.fit(seq)
    model4 = GaussianHMM(n_components=14, random_state=42, n_iter=300)
    seq=[[i] for i in m]
    model4.fit(seq)
    model5 = GaussianHMM(n_components=13, random_state=888, n_iter=300)
    seq=[[i] for i in r]
    model5.fit(seq)
    models=[model1,model2,model3,model4, model5]
    with open(output_file, "w") as f:
        f.write("file, dev_classified \n")



    for file_name in os.listdir(test_folder):
        data = pd.read_csv(os.path.join(test_folder, file_name))
        d = data['dev'].values
        arr=[]
        for m in models:
            arr.append(m.score([[i] for i in d]))
        device=devs[arr.index(max(arr))]
        with open(output_file, "a") as f:
            f.write(file_name+", "+device+"\n")




def main():
    parser = argparse.ArgumentParser(description="Classify devices based on given data.")
    parser.add_argument("--train", help="Path to the training data file.", required=True)
    parser.add_argument("--test", help="Path to the folder containing test data.", required=True)
    parser.add_argument("--output", help="Path to the output file.", default="results.txt")
    args = parser.parse_args()

    classify_devices(args.train, args.test, args.output)

if __name__ == "__main__":
    main()
