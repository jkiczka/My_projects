import tkinter as tk
from tkinter import messagebox
from tkinter import *
from tkinter.ttk import Combobox
import mysql.connector
import ctypes
import datetime
from test import mail


class Mylibrary:
    def __init__(self, root, userid):
        self.root = root
        self.user_id=userid
        self.root.title("Library")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        self.root['bg']='#D2EFF2'

        self.bbooks_database = []
        self.rbooks_database = []
        self.load_borrowed()
        self.load_reserved()
        
        # Dodanie pola wyszukiwania

        self.text_widget = Label(self.root, height=1, text="My borrowed books:", bg='#D2EFF2', font=("tahoma", 20, "bold"))
        self.text_widget.place(x=70,y=100)

        self.text_widget = Label(self.root,  height=1, text="My reservations:", bg='#D2EFF2', font=("tahoma", 20, "bold"))
        self.text_widget.place(x=1210,y=100)


        

        # Tworzenie listbox'a do wyświetlenia listy wypozyczonych ksiazek
        bbook_listbox = Listbox(self.root, selectmode=SINGLE, width=80, height=20)
        bbook_listbox.place(x=50, y=200)
        self.bbook_listbox=bbook_listbox
       
        
        
        #zarezerwowane ksiazki moje
        rbook_listbox = Listbox(self.root, selectmode=SINGLE, width=80, height=20)
        rbook_listbox.place(x=1200, y=200)
        self.rbook_listbox=rbook_listbox
        self.refresh_book_list()
   
     
        
       
        button = Button(self.root, text="Back to library main page", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.back_to_library)
        button.place(x=1000, y=1200)




        

        # Dodawanie eventu przewijania myszą do listboxa
        def on_mousewheel(event):
            self.bbook_listbox.yview_scroll(-1 * int(event.delta/120), "units")
            self.rbook_listbox.yview_scroll(-1 * int(event.delta/120), "units")
        self.bbook_listbox.bind("<MouseWheel>", on_mousewheel)
        self.rbook_listbox.bind("<MouseWheel>", on_mousewheel)

        


        # Tworzenie kontenera (frame) dla przycisków
        buttons_frame = Frame(root, bg='#D2EFF2', width=500, height=500)
        buttons_frame.place(x=300, y=900)

        # Dodawanie przycisku do wypożyczania książki
        borrow_button = Button(buttons_frame, text="Return book", command=self.return_book, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        borrow_button.pack(side=LEFT, padx=0)

        reserve_button = Button(buttons_frame, text="Pick up reservation", command=self.reserve_pickup, bg='#8cc4e6', font=("tahoma", 15, "bold"))
        reserve_button.pack(side=LEFT, padx=1000)


    def back_to_library(self):
        self.root.destroy()
        self.root=Tk()
        self.app=CE_Window(self.root, self.user_id)

    def load_borrowed(self):
        try:
            connection = mysql.connector.connect(
                host="localhost",
                user="root",
                password="1234",
                database="mydb"
            )
            cursor = connection.cursor()

            # Pobranie danych z tabeli books
            query = "SELECT * from borrowed WHERE user_id=%s"
            data=(self.user_id,)
            cursor.execute(query, data)
            
            books_data = cursor.fetchall()
            self.bnum_rows = cursor.rowcount

            # Konwersja danych do formatu books_database
            self.bbooks_database = []
            for book_data in books_data:
                book = {
                    "id": book_data[0],
                    "title": book_data[1],
                    "author": book_data[2],
                    "year published": book_data[3],
                    "publisher":book_data[4],
                    "category":book_data[5]
                }
                self.bbooks_database.append(book)

        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def load_reserved(self):
            try:
                connection = mysql.connector.connect(
                    host="localhost",
                    user="root",
                    password="1234",
                    database="mydb"
                )
                cursor = connection.cursor()

                # Pobranie danych z tabeli books
                query = "SELECT * from reserved WHERE user_id=%s and reserved.end>CURDATE();"
                data=(self.user_id,)
                cursor.execute(query, data)
                
                books_data = cursor.fetchall()
                self.rnum_rows = cursor.rowcount

                # Konwersja danych do formatu books_database
                self.rbooks_database = []
                for book_data in books_data:
                    book = {
                        "id": book_data[0],
                        "title": book_data[1],
                        "author": book_data[2],
                        "year published": book_data[3],
                        "publisher":book_data[4],
                        "category":book_data[5]
                    }
                    self.rbooks_database.append(book)

            except mysql.connector.Error as error:
                print("Błąd połączenia z bazą danych:", error)
            finally:
                if connection.is_connected():
                    cursor.close()
                    connection.close()
   
 

    def return_book(self):
        
            selected_index = self.bbook_listbox.curselection()
            if selected_index:
                book = self.bbooks_database[selected_index[0]]
                self.update_book(book["id"],2)
                self.refresh_book_list()
                messagebox.showinfo( "Success", f"Book '{book['title']}' is successfully returned.")
        


            


    
    def reserve_pickup(self):
        if self.bnum_rows<5:
            selected_index = self.rbook_listbox.curselection()
            if selected_index:
                book = self.rbooks_database[selected_index[0]]
                self.update_book(book["id"],1)
                self.refresh_book_list()
                messagebox.showinfo( "Success", f"Book '{book['title']}' is successfully borrowed.")
        else:
            messagebox.showinfo( "Error", f"You can only borrow 5 books at a time. \n \n Before picking up your reservation,\n return at least one borrowed book")
            

    def update_book(self,book_id,type):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            query="SET SQL_SAFE_UPDATES = 0;"
            cursor.execute(query)

            if(type==1): # reserve pickup - dodaj wpis do borrowing, usun rezerwacje
                query = "INSERT INTO borrowing (start, end, returned, book_copy_id, user_id) values (CURDATE(),NULL,0,%s,%s);"
                data = (book_id, str(self.user_id))
                cursor.execute(query, data)
                #query = "UPDATE reservation SET reservation.end=NULL where reservation.book_copy_id=%s;"
                query="DELETE FROM RESERVATION where reservation.book_copy_id=%s"
                data = (book_id,)
                cursor.execute(query, data)
                
            if(type==2): #return - dodaj date oddania do borrowing i ustaw return na 1
                query = "UPDATE borrowing SET borrowing.end=CURDATE(), borrowing.returned=1 WHERE borrowing.book_copy_id=%s;"
                data = (book_id,)
                cursor.execute(query, data)
                
            query='SELECT bookinfo.title FROM bookinfo where bookinfo.id=%s'
            data=(book_id,)
            cursor.execute(query, data)
            title=cursor.fetchone()

            query='SELECT users.email FROM users where user.id=%s'
            data=(str(self.user_id),)
            cursor.execute(query, data)
            reciever=cursor.fetchone()

            mail(reciever,3,title)


            connection.commit()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
            
              


    
         
            

    def refresh_book_list(self):
        self.bbook_listbox.delete(0, END)
        self.rbook_listbox.delete(0, END)
        self.load_borrowed()
        self.load_reserved()
        for book in self.bbooks_database:
                self.bbook_listbox.insert(END, f"{book['title']} - {book['author']} - {book['year published']} - {book['publisher']} - {book['category']}")
        for book in self.rbooks_database:
                self.rbook_listbox.insert(END, f"{book['title']} - {book['author']} - {book['year published']} - {book['publisher']} - {book['category']}")

    


def valid_author(x):
     if(x!=""):
          return True
     else:
          return False

class Addnewbook():
    def __init__(self, root, userid):
        self.root = root
        self.user_id=userid
        self.root.title("Add new coursebook")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        self.root['bg']='#D2EFF2'

        self.authors=[] #tylko wybrani
        self.inittitle="                               - - -                               "
        self.selected_title=""
        self.selected_category=""
        self.selected_publisher=""
        self.title=self.inittitle
        self.publisher=self.inittitle
        authors = self.get_authors() #wszyscy
        self.author_var = StringVar()
        author_combobox = Combobox(self.root, textvariable=self.author_var, values=authors, width=40)
        author_combobox.place(x=1400, y=120)
        author_combobox.bind("<<ComboboxSelected>>", self.on_author_selected)

        titles = self.get_titles()
        self.title_var = StringVar()
        title_combobox = Combobox(self.root, textvariable=self.title_var, values=titles, width=80)
        title_combobox.place(x=20, y=360)
        title_combobox.bind("<<ComboboxSelected>>", self.on_title_selected)

        publishers = self.get_publishers()
        self.publisher_var = StringVar()
        publisher_combobox = Combobox(self.root, textvariable=self.publisher_var, values=publishers, width=80)
        publisher_combobox.place(x=20, y=960)
        publisher_combobox.bind("<<ComboboxSelected>>", self.on_publisher_selected)

        main_label=Label(self.root, text="Add a coursebook to the library", font=("tahoma",15, "bold"), bg='#D2EFF2',fg='#167df3', width=50)
        main_label.place(x=0, y=30)
        self.mainlabel=main_label

        main_label=Label(self.root, text="Add a coursebook to the library", font=("tahoma",15, "bold"), bg="#D2EFF2",fg='black', width=50)
        main_label.place(x=0, y=31)
        self.mainlabel=main_label

        main_label=Label(self.root, text="Fill all important data - select options from dropdown or insert data manually", fg='#1772b3', font=("tahoma",10, "bold"), bg='#D2EFF2', width=70 )
        main_label.place(x=10, y=90)
        self.mainlabel=main_label


        add_button = Button(self.root, text="Add author", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.add)
        add_button.place(x=2100, y=120)
        d_button = Button(self.root, text="Delete author", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.delete)
        d_button.place(x=2100, y=190)

        title_label=Label(self.root, text="Title:", font=("tahoma",10), bg='#D2EFF2' )
        title_label.place(x=20, y=210)
        self.titlelabel=title_label

        title_label=Label(self.root, text=self.inittitle, font=("tahoma",10), bg='white', relief='solid', borderwidth=1, width=70)
        title_label.place(x=100, y=215)
        self.titlelabel=title_label

        stitle_label=Label(self.root, text="Select title or type new one below", font=("tahoma",10), bg='#D2EFF2' )
        stitle_label.place(x=20, y=300)
        self.stitlelabel=stitle_label


        titleEntry=Entry(root, width=63, font=("tahoma",12), bg="white", fg="black", bd=1)
        titleEntry.place(x=20, y=420)
        self.titleEntry=titleEntry

        author_label=Label(self.root, text="Author:", font=("tahoma",10), bg='#D2EFF2' )
        author_label.place(x=1400, y=70)
        self.authorlabel=author_label

        authors_label=Label(self.root, text="Selected authors:", font=("tahoma",10), bg='#D2EFF2' )
        authors_label.place(x=1400, y=300)
        self.authorslabel=authors_label

        authors_label=Label(self.root, text="Select author above or type beow:", font=("tahoma",10), bg='#D2EFF2' )
        authors_label.place(x=1400, y=170)
        self.authorslabel=authors_label

        

        

        authorEntry=Entry(root, width=32, font=("tahoma",12), bg="white", fg="black", bd=1)
        authorEntry.place(x=1400, y=220)
        self.authorEntry=authorEntry

        author_listbox = Listbox(self.root, selectmode=SINGLE, width=40, height=20)
        author_listbox.place(x=1400, y=350)
        self.author_listbox=author_listbox
        
        adt_button = Button(self.root, text="Confirm self-defined title", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.confirmtitle)
        adt_button.place(x=10, y=500)

        dt_button = Button(self.root, text="Delete self-defined title", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.deletetitle)
        dt_button.place(x=400, y=500)


        cat_label=Label(self.root, text="Category: ", font=("tahoma",10), bg='#D2EFF2' )
        cat_label.place(x=20, y=600)
        self.catlabel=cat_label


        catEntry=Entry(root, width=63, font=("tahoma",12), bg="white", fg="black", bd=1 )
        catEntry.place(x=20, y=650)
        self.catEntry=catEntry

        year_label=Label(self.root, text="Year:", font=("tahoma",10), bg='#D2EFF2' )
        year_label.place(x=20, y=710)
        self.yearlabel=year_label


        yearEntry=Entry(root, width=63, font=("tahoma",12), bg="white", fg="black", bd=1 )
        yearEntry.place(x=20, y=760)
        self.yearEntry=yearEntry

        pub_label=Label(self.root, text="Publisher:", font=("tahoma",10), bg='#D2EFF2' )
        pub_label.place(x=20, y=850)
        self.publabel=pub_label

        spub_label=Label(self.root, text=self.inittitle, font=("tahoma",10), bg='white', relief='solid', borderwidth=1, width=50)
        spub_label.place(x=160, y=850)
        self.spublabel=spub_label

        npub_label=Label(self.root, text="Select publisher or type new one below", font=("tahoma",10), bg='#D2EFF2' )
        npub_label.place(x=20, y=900)
        self.npublabel=spub_label


        pubEntry=Entry(root, width=63, font=("tahoma",12), bg="white", fg="black", bd=1)
        pubEntry.place(x=20, y=1020)
        self.pubEntry=pubEntry

        

        pc_button = Button(self.root, text="Confirm self-defined publisher", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.confirmpublisher)
        pc_button.place(x=10, y=1100)

        dp_button = Button(self.root, text="Delete self-defined publisher", bg='#5daad9',font=("tahoma", 10, "bold"),command=self.deletepublisher)
        dp_button.place(x=500, y=1100)


        confirm_button = Button(self.root, text="Confirm all data",fg="white", bg='#c4292c',font=("tahoma", 15, "bold"),command=self.confirmall)
        confirm_button.place(x=1900, y=1200)

        confirm_button = Button(self.root, text="Back to main library page",fg="black", bg='#5daad9',font=("tahoma", 15, "bold"),command=self.backtolibrary)
        confirm_button.place(x=1200, y=1200)

        


       

        self.refresh()

    def backtolibrary(self):
        self.root.destroy()
        self.root=Tk()
        self.app=CE_Window(self.root, self.user_id)

    def insert_book(self,title,category):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            data=(title,category)

            query="INSERT INTO book(title,category_id) values (%s, %s)"

            cursor.execute(query,data)
            
            connection.commit()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def get_book_id(self,data):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            query="SELECT id from book where title=%s"

            data=(data,)
            cursor.execute(query,data)
            
            x=cursor.fetchone()
            if x:
                x=x[0]
            connection.commit()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
                return x
            

    def get_category_id_with_name(self,data):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            query="SELECT id from category where name=(%s)"

            data=(data,)

            cursor.execute(query,data)
            x=cursor.fetchone()

            if x:
                x=x[0]

            connection.commit()
            
        except mysql.connector.Error as error:
            x=None
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
                return x
   
    def insert_category_with_name(self,data):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()
            data=(data,)
            query="INSERT INTO category(name) values (%s)"

            cursor.execute(query,data)
            
            connection.commit()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()


    def insert_bookauthor(self,book_id,a):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()
            query="INSERT INTO book_author(book_id,author_id) values (%s, %s)"
            data=(book_id,a)
            cursor.execute(query,data)
            connection.commit()
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()


    def insert_author(self,a):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()
            a=(a,)
            query="INSERT INTO author(name) values (%s)"
            cursor.execute(query,a)
            connection.commit()
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def insert_publisher(self,p):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()
            data=(p,)
            query="INSERT INTO publisher(name) values (%s)"
            cursor.execute(query,data)
            connection.commit()
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def get_publisher_with_name(self,name):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()
            name=(name,)
            query="SELECT id from publisher where name=(%s)"

            cursor.execute(query,name)
            
            x=cursor.fetchone()
            if x:
                x=x[0]
            connection.commit()
            
        except mysql.connector.Error as error:
            x=None
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
                return x
   
    def get_author_id_with_name(self,a):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            a=(a,)

            query="SELECT id from author where name=(%s)"

            cursor.execute(query,a)
            
            x=cursor.fetchone()

            if x:
                x=x[0]
            connection.commit()
            
        except mysql.connector.Error as error:
            x=None
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
                return x
            
    def insert_book_copy(self,year,id,publisher):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

            data=(year,id,publisher)

            query="INSERT INTO book_copy(year_published,book_id,publisher_id) values (%s,%s,%s)"

            cursor.execute(query,data)

            x=cursor.fetchone()
            connection.commit()

            
            
        except mysql.connector.Error as error:
            x=None
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
                return x

    def confirmall(self):
        if self.title!=self.inittitle: #wpisano tytul
            self.selected_category=self.catEntry.get() #when book title is self defined, category is also self defined
            category_id=self.get_category_id_with_name(self.selected_category)
            if (not category_id):
                self.insert_category_with_name(self.selected_category)
                category_id=self.get_category_id_with_name(self.selected_category)
            self.insert_book(self.title, category_id)
            book_title=self.title
        else:
            book_title=self.selected_title.split(" - ")[0][:-1] #tu akuart po tytule byla spacja na koncu, nwm czemu
            print(book_title)
        
        
        book_id=self.get_book_id(book_title)
        
        print(book_id)
        list_of_authors_id=[]

        for a in self.authors:
             if self.get_author_id_with_name(a)==None:
                 self.insert_author(a)
             list_of_authors_id.append(self.get_author_id_with_name(a))

        print(list_of_authors_id)

        for a in list_of_authors_id:
             self.insert_bookauthor(book_id,a)
        
        if self.publisher!=self.inittitle: #wpisano tytul
             print(type(self.publisher))
             self.insert_publisher(self.publisher)
             book_publisher=self.publisher
        else:
             book_publisher=self.selected_publisher
        publisher_id=self.get_publisher_with_name(book_publisher)
        year=self.yearEntry.get()

        self.insert_book_copy(year, book_id, publisher_id)

        
                


        
        
        #
        '''
        
        wprowadzic rozroznienie miedzy zwyklym title(wpisanym), a selected_title (wybranym)
        i tak samo dla innych, sprawdzic czy wpisany rzeczywiscie jeszcze nie istnieje w bazie
        title=self.title
        publisher=self.selected_publisher
        *Book(id,title,category_id) DONE
        *Author(id, name) DONE
        *Book_author(book_id, author_id) DONE
        *Category(id, name) DONE 
        *Publisher(id,name)
        *Book_copy(id, year_published, book_id, publisher_id)
        '''
        pass
         

    def on_author_selected(self, event):
        self.authors.append(self.author_var.get())
        
        

    def get_authors(self):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

                
            query='SELECT author.name FROM author'
            cursor.execute(query)
            authors=cursor.fetchall()
            aut=set()
            for i in authors:
                aut.add(i[0])

            connection.commit()

            result=list(aut)

            result.sort()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
            return result
        
    def on_publisher_selected(self, event):
        self.selected_publisher = self.publisher_var.get()
        self.npublabel.config(text=self.selected_publisher)
        self.refresh()
        

    def get_publishers(self):
        try:
            connection = mysql.connector.connect(
                host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                user="root",  # Zmień na nazwę użytkownika bazy danych
                password="1234",  # Zmień na hasło użytkownika bazy danych
                database="mydb"  # Zmień na nazwę Twojej bazy danych
            )
            cursor = connection.cursor()

                
            query='SELECT publisher.name FROM publisher'
            cursor.execute(query)
            authors=cursor.fetchall()
            aut=set()
            for i in authors:
                aut.add(i[0])

            connection.commit()

            result=list(aut)

            result.sort()
            
        except mysql.connector.Error as error:
            print("Błąd połączenia z bazą danych:", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
            return result
        
    def on_title_selected(self, event):
            self.selected_title = self.title_var.get()
            self.titlelabel.config(text=self.selected_title)
            self.selected_category=self.selected_title.split(" - ")[1]
            self.catEntry.delete(0, END)
            self.catEntry.insert(0,self.selected_category[1:])
            self.refresh()

    def get_titles(self):
            try:
                connection = mysql.connector.connect(
                    host="localhost",  # Zmień na adres hosta Twojej bazy danych MySQL
                    user="root",  # Zmień na nazwę użytkownika bazy danych
                    password="1234",  # Zmień na hasło użytkownika bazy danych
                    database="mydb"  # Zmień na nazwę Twojej bazy danych
                )
                cursor = connection.cursor()

                    
                query='SELECT bookinfo.title,bookinfo.category FROM bookinfo'
                cursor.execute(query)
                authors=cursor.fetchall()
                aut=set()
                for i in authors:
                    aut.add(i[0]+"  -  "+i[1])

                result=list(aut)
                result.sort()

                connection.commit()
                
            except mysql.connector.Error as error:
                print("Błąd połączenia z bazą danych:", error)
            finally:
                if connection.is_connected():
                    cursor.close()
                    connection.close()
                return result
            
            
    def add(self): #adding self-defined authors to self.authors, refresh will add them to combobox
        author = self.authorEntry.get()
        if author and author not in self.authors:
            self.authors.append(author)
        self.refresh()
        

    def delete(self):
        self.authors.pop()
        self.refresh()

    def deletetitle(self):
         self.title=self.inittitle
         self.titlelabel.config(text=self.title)
         self.refresh()

    def confirmtitle(self):
         self.title=self.titleEntry.get()
         self.selected_category=""
         self.titlelabel.config(text=self.title)

    
    def deletepublisher(self):
         self.publisher=self.inittitle
         self.npublabel.config(text=self.publisher)
         self.refresh()

    def confirmpublisher(self):
         self.publisher=self.pubEntry.get()
         self.npublabel.config(text=self.publisher)
         self.refresh()

    

    def refresh(self):
        
        self.author_listbox.delete(0, END)
        for a in self.authors:
                self.author_listbox.insert(END, a)
        
        self.authorEntry.config(textvariable="")

        
        

        
        
        

if __name__ == "__main__":
    ctypes.windll.shcore.SetProcessDpiAwareness(1)
    root=tk.Tk()
    app = Addnewbook(root,1)
    
    root.mainloop()


