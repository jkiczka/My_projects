import tkinter as tk
from tkinter import messagebox
from tkinter import *
from tkinter.ttk import Combobox
import mysql.connector
import ctypes
import datetime
from email.message import EmailMessage
import ssl
import smtplib

def mail(email_receiver,type,title):
    email_sender='librarjany@gmail.com'

    email_password='chvbipxrnjzwyhxi'

    if(type==1):# borrowing
        subject='Library - borrowing'
        current_date=datetime.date.today()+datetime.timedelta(days=30)
        body="Book "+ title +" was succesfuly borrrowed. Due date is "+current_date
    elif(type==2): #reserve
        subject='Library - reservation'
        current_date=datetime.date.today()+datetime.timedelta(days=7)
        body="Your reservation for book "+ title + " is successful. You can pick it up before " + current_date
    elif(type==3): #add to wishlist
        subject='Library - wishlist'
        body="You were on the wishlist for "+ title +" book. Now it is available. You can make reservation or borrow it."

    em=EmailMessage()
    em['From']=email_sender
    em['To']=email_receiver
    em['Subject']=subject
    em.set_content(body)

    context=ssl.create_default_context()

    with smtplib.SMTP_SSL('smtp.gmail.com', 465, context=context) as smtp:
        smtp.login(email_sender,email_password)
        smtp.sendmail(email_sender,email_receiver, em.as_string())


class LiBorrow:
    def __init__(self, root, userid):
        self.root = root
        self.user_id=userid
        self.root.title("Library")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        self.root['bg']='#D2EFF2'

        self.books_database = []
        self.load_books_from_database()
        
        # Dodanie pola wyszukiwania

        self.text_widget = Label(self.root, width=10, height=1, text="Title:", bg='#D2EFF2', font=("tahoma", 10, "bold"))
        self.text_widget.place(x=0,y=40)

        search_entry = Entry(self.root,  width=60, font=("tahoma", 11))
        search_entry.place(x=150, y=40)
        search_entry.bind("<KeyRelease>", lambda event: self.refresh_book_list(search_entry.get()))

        

        # Tworzenie listbox'a do wyświetlenia listy książek
        book_listbox = Listbox(self.root, selectmode=SINGLE, width=200, height=20)
        book_listbox.place(x=10, y=200)
        self.book_listbox=book_listbox
        self.refresh_book_list()
        

        authors = self.get_unique_authors()
        self.author_var = tk.StringVar()
        author_combobox = Combobox(self.root, textvariable=self.author_var, values=authors)
        author_combobox.place(x=1420, y=40)
        author_combobox.bind("<<ComboboxSelected>>", self.on_author_selected)
        

        category = self.get_unique_category()
        self.category_var = tk.StringVar()
        category_combobox = Combobox(self.root, textvariable=self.category_var, values=category)
        category_combobox.place(x=2020, y=40)
        category_combobox.bind("<<ComboboxSelected>>", self.on_category_selected)
        

        filter_button = Button(self.root, text="Filter results", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.apply_filters)
        filter_button.place(x=2100, y=120)
        

        self.text_widget = Label(self.root, width=10, height=1, text="Author:", bg='#D2EFF2', font=("tahoma", 10, "bold"))
        self.text_widget.place(x=1240,y=40)

        self.text_widget = Label(self.root, width=10, height=1, text="Category:", bg='#D2EFF2', font=("tahoma", 10, "bold"))
        self.text_widget.place(x=1830,y=40)
       
       

        # Tworzenie suwaczka (Scrollbar) i powiązanie go z listboxem
        scrollbar = Scrollbar(root, orient=VERTICAL, command=self.book_listbox.yview)
        scrollbar.pack(side=RIGHT, fill=Y)
        self.book_listbox.config(yscrollcommand=scrollbar.set)

        # Dodawanie eventu przewijania myszą do listboxa
        def on_mousewheel(event):
            self.book_listbox.yview_scroll(-1 * int(event.delta/120), "units")
        self.book_listbox.bind("<MouseWheel>", on_mousewheel)

        #self.refresh_book_list()


        # Tworzenie kontenera (frame) dla przycisków
        buttons_frame = Frame(root, bg='#D2EFF2', width=500, height=500)
        buttons_frame.place(x=300, y=1000)

        # Dodawanie przycisku do wypożyczania książki
        borrow_button = Button(buttons_frame, text="Borrow", command=self.borrow_book, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        borrow_button.pack(side=LEFT, padx=50)

        reserve_button = Button(buttons_frame, text="Reserve", command=self.reserve, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        reserve_button.pack(side=LEFT, padx=50)


        # Dodawanie przycisku do dodawania książki do listy życzeń
        wishlist_button = Button(buttons_frame, text="Add to wishlist", command=self.add_to_wishlist, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        wishlist_button.pack(side=LEFT, padx=50)

        back_button = Button(buttons_frame, text="Back to libarary main page", command=self.back_to_library, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        back_button.pack(side=LEFT, padx=50)

    def load_books_from_database(self):
        try:
            connection = mysql.connector.connect(
                host="localhost",
                user="root",
                password="1234",
                database="mydb"
            )
            cursor = connection.cursor()

            # Pobranie danych z tabeli books
            query = "SELECT * from main_view"
            cursor.execute(query)
            books_data = cursor.fetchall()

            # Konwersja danych do formatu books_database
            self.books_database = []
            for book_data in books_data:
                book = {
                    "id": book_data[0],
                    "title": book_data[1],
                    "author": book_data[2],
                    "year published": book_data[3],
                    "publisher":book_data[4],
                    "category":book_data[5],
                    "available": bool(book_data[6])
                }
                self.books_database.append(book)

        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def apply_filters(self):
        selected_author = self.author_var.get()
        selected_category = self.category_var.get()
        self.refresh_book_list(author=selected_author, category=selected_category)

    def get_unique_authors(self):
        authors = {book['author'] for book in self.books_database}
        return list(authors)

    def on_author_selected(self, event):
        self.selected_author = self.author_var.get()
        
    def get_unique_category(self):
        category = {book['category'] for book in self.books_database}
        return list(category)

    def on_category_selected(self, event):
        self.selected_category = self.category_var.get()
        

    def add_to_wishlist(self):
        selected_index = self.book_listbox.curselection()
        if selected_index:
            
            book = self.books_database[selected_index[0]]
            if not book["available"]:
                k=self.update_book(book["id"],3)
                if(k==0):
                    messagebox.showinfo( "Success", f"Book '{book['title']}' is successfully added to your wishlist. \n You will receive an email when it will be available")
                
            else:
                messagebox.showwarning("Error", f"Book '{book['title']}' is available. You can make a reservation")
            self.refresh_book_list()


    def back_to_library(self):
        self.root.destroy()
        self.root=Tk()
        self.app=CE_Window(self.root, self.user_id)
    
    
    def reserve(self):
        selected_index = self.book_listbox.curselection()
        if selected_index:
            
            book = self.books_database[selected_index[0]]
            if book["available"]:
                k=self.update_book(book["id"],2)
                current_date=datetime.date.today()+datetime.timedelta(days=7)
                if(k==0):
                    messagebox.showinfo( "Success", f"Book '{book['title']}' is successfully rerserved. \n\n Due date: {current_date}")
                
            else:
                messagebox.showwarning("Error", f"Book '{book['title']}' is unavailable. You can try to add it to your wishlist")
            self.refresh_book_list()




    def borrow_book(self): 
        selected_index = self.book_listbox.curselection()
        if selected_index:
            book = self.books_database[selected_index[0]]
   
            if book["available"]:
                k=self.update_book(book["id"],1)
                current_date=datetime.date.today()+datetime.timedelta(days=30)
                
                if(k==0):
                    messagebox.showinfo( "Success", f"Book '{book['title']}' is successfully borrowed. \n\n Return date: {current_date}")
                
            else:
                messagebox.showwarning("Błąd", f"Book '{book['title']}' is unavailable. You can try to add it to your wishlist")
            self.refresh_book_list()

    # Funkcja do odświeżania listy książek
    def refresh_book_list(self,search_text=None, author=None, category=None):
        self.book_listbox.delete(0, END)
        self.load_books_from_database()
        for book in self.books_database:
            if (not search_text or search_text.lower() in book['title'].lower()) and \
               (not author or book['author'] == author) and \
               (not category or book['category'] == category) :
                book_status = "Available" if book["available"] else "Unavailable"
                self.book_listbox.insert(END, f"{book['title']} - {book['author']} - {book['year published']} - {book['publisher']} - {book['category']} ({book_status})")
        
    # Funkcja do aktualizacji dostępności książki w bazie danych
    def update_book(self,book_id,type):
        k=0
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            if(type==1):# borrowing
                query = "SELECT * from borrowing WHERE user_id=%s and returned=0"
                data=(self.user_id,)
                cursor.execute(query, data)
                cursor.fetchall()
                if(cursor.rowcount<5):
                    query = "INSERT INTO borrowing (start, end, returned, book_copy_id, user_id) values  (CURDATE(),NULL,0,%s,%s);"
                    data = (book_id, str(self.user_id))
                    cursor.execute(query, data)

                    query='SELECT bookinfo.title FROM bookinfo where bookinfo.id=%s'
                    data=(book_id,)
                    cursor.execute(query, data)
                    title=cursor.fetchone()

                    query='SELECT users.email FROM users where user.id=%s'
                    data=(str(self.user_id),)
                    cursor.execute(query, data)
                    reciever=cursor.fetchone()

                    mail(reciever,1,title)
                    #today-> email with due date 30
                else:
                    k=1
                    messagebox.showerror("Error", "You can only borrow 5 books in a while.")
            if(type==2): #reserve
                query = "SELECT * from reserved WHERE user_id=%s and end>=NOW()"
                data=(self.user_id,)
                cursor.execute(query, data)
                cursor.fetchall()
                if(cursor.rowcount<5):
                    query = "INSERT INTO reservation(start, end, book_copy_id, user_id) values  (CURDATE(),CURDATE()+INTERVAL 7 DAY,%s,%s);"
                    data = (book_id, str(self.user_id))
                    cursor.execute(query, data)

                    query='SELECT bookinfo.title FROM bookinfo where bookinfo.id=%s'
                    data=(book_id,)
                    cursor.execute(query, data)
                    title=cursor.fetchone()

                    query='SELECT users.email FROM users where user.id=%s'
                    data=(str(self.user_id),)
                    cursor.execute(query, data)
                    reciever=cursor.fetchone()

                    mail(reciever,2,title)
                else:
                    k=1
                    messagebox.showerror("Error", "You can only reserve 5 books in a while.")
               
            if(type==3): #add to wishlist
                print('inserted')
                query = "SELECT * FROM wishlist WHERE book_copy_id=%s;"
                data=(book_id,)
                cursor.execute(query,data)
                row=cursor.fetchone()
                if row!=None:
                    messagebox.showerror("Error", "Someone is already waiting for this book. Try again in a while.")
                    k=1
                    
                else:
                    query = "INSERT INTO wishlist(user_id,book_copy_id) VALUES (%s, %s);"
                    data=(self.user_id,book_id)
                    cursor.execute(query,data)
                    

                    
            query2="INSERT INTO notification(sent_at,type,user_id, book_copy_id) values (CURDATE(),%s,%s, %s);"
            data2=(type,self.user_id,book_id)
            cursor.execute(query2, data2)
            connection.commit()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
            return k

    

if __name__ == "__main__":
    ctypes.windll.shcore.SetProcessDpiAwareness(1)
    root=tk.Tk()
    app = LiBorrow(root,1)
    root.mainloop()
