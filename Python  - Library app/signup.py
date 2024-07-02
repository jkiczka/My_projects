from tkinter import * 
from tkinter import messagebox
import ctypes
from tkinter import ttk
from PIL import Image, ImageTk
import re
import mysql.connector
import bcrypt
import os
from test import mail, LiBorrow 
from myaccount import Mylibrary

from tkinter.ttk import Combobox
import ctypes
import datetime
from email.message import EmailMessage
import ssl
import smtplib




def generate_salt():
    return bcrypt.gensalt()

def generate_pepper():
    return os.urandom(16)

def hash_password(password):
    salt = generate_salt()  # Generuje losową sól
    pepper = generate_pepper()  # Generuje losowy pieprz (16 bajtów jako przykład)
    hashed_password = bcrypt.hashpw(password.encode('utf-8') + pepper, salt)
    return hashed_password, salt, pepper


def verify_password(hashed_password, input_password, salt, pepper):
    hashed_input_password = bcrypt.hashpw(input_password.encode('utf-8') + pepper, salt)
    return hashed_password == hashed_input_password

import pickle

# Funkcja do zapisywania pary (<string>, <bytes>) do pliku
def save_pair_to_file(filename, string_data, binary_data):
    pair_to_save = (string_data, binary_data)

    with open(filename, 'wb') as file:
        pickle.dump(pair_to_save, file)

# Funkcja do odczytywania pary (<string>, <bytes>) z pliku binarnego na podstawie stringa
def find_pair_by_string(filename, search_string):
    with open(filename, 'rb') as file:
        while True:
            try:
                loaded_pair = pickle.load(file)
                if loaded_pair[0] == search_string:
                    return loaded_pair[1]
            except EOFError:
                # Wyjątek EOFError oznacza koniec pliku
                break

    return None


class Login_Window:
    

    def __init__(self,root):
        self.root=root
        self.root.title("Login")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        #self.root['bg']='#D2EFF2'
        bgimage=ImageTk.PhotoImage(file='tlo.jpg')
        self.bgimage=bgimage
        bgLabel=Label(self.root, image=bgimage)
        bgLabel.grid(row=0,column=0)

        frame=Frame(self.root, bg="white")
        frame.place(x=860,y=250, width=680, height=900)

        img1=Image.open(r"C:\Users\PC\Desktop\login_form\logo.png")
        img1=img1.resize((200,200),Image.ANTIALIAS)
        self.photoimage1=ImageTk.PhotoImage(img1)
        lblimg1=Label(image=self.photoimage1, bg="white", borderwidth=0)
        lblimg1.place(x=1100, y=300, width=200, height=200)

        get_str=Label(frame, text="user login", font=("tahoma", 20, "bold"), fg="#3A949E", bg="white")
        get_str.place(x=210, y=280)
        
        
        usernameEntry=Entry(root, width=10, font=("tahoma",10), bg="white", fg="#3A949E", bd=0)
        usernameEntry.place(x=1100, y=650)
        usernameEntry.insert(0,'username')
        self.log=usernameEntry
        self.log.bind('<FocusIn>', self.on_username)
        self.frame1=Frame(self.root, width=220, height=2, bg="#3A949E")
        self.frame1.place(x=1100, y=690)

        passwEntry=Entry(root, width=10, font=("tahoma",10), bg="white", fg="#3A949E", bd=0)
        passwEntry.place(x=1100, y=750)
        passwEntry.insert(0,'password')
        self.passw=passwEntry
        self.passw.bind('<FocusIn>', self.on_passw)
        self.frame2=Frame(self.root, width=220, height=2, bg="#3A949E")
        self.frame2.place(x=1100, y=790)
        
        
        openeye=PhotoImage(file=r"openeye2.png")
        self.eyebutton=openeye
        eyeButton=Button(self.root, image=openeye, height=30, width=30, bd=0, bg='white', activebackground='white', command=self.hide, cursor='hand2')
        eyeButton.place(x=1280,y=755)
        self.eyebutton2=eyeButton

        
        loginButton=Button(self.root, text='login', font=('Open Sans', 20, 'bold'), fg='white', bg='#3A949E', bd=0, cursor='hand2', command=self.login)
        loginButton.place(x=1110,y=880)
        self.loginbutton=loginButton


        register=Button(self.root, text='Create new account',font=('Open Sans', 10), bg='white', fg='#9CBABF', bd=0, cursor='hand2', command=self.register)
        register.place(x=1255,y=1085)
        self.registerbutton=register
    
    def register(self):
        self.root.destroy()
        self.root=Tk()
        self.app=Signup_Window(self.root)
        
  
    def login(self):
        if self.log.get()=="" or self.passw.get()=="":
            messagebox.showerror("Error", "All fields are required")
        else:
            conn=mysql.connector.connect(host="localhost", user="root", password="1234", database="mydb")
            my_cursor=conn.cursor()
            '''
            1)znajdz uzytkownika o tym samym username i odczytaj jego zahaszowane haslo i sol z bd
            2)odczytaj pieprz dla tego usera z pliku
            3) zahashuj wpisane haslo ze znalezionymi pieprzem i sola, jak sie zgadza z haslem z bd -> zaloguj
            
            '''
            my_cursor.execute("select password, salt, id from users where username=%s",(self.log.get(),))
            row=my_cursor.fetchone()
            if row==None:
                messagebox.showerror("Error", "Invalid username & passsword")
            else:
                #odczytaj pieprz dla usera z pliku
                hashedpw=row[0]
                salt=row[1]
                pepper=find_pair_by_string("pepper.txt",self.log.get())
                if(verify_password(hashedpw,self.passw.get(),salt, pepper)):
                    self.root.destroy()
                    self.root=Tk()
                    self.app=Start_Window(self.root, row[2]) #####################################################################################################################################################################################
                else:
                    messagebox.showerror("Error", "Invalid username & passsword")
            conn.commit()
            conn.close()


    
    def hide(self):
        self.eyebutton.config(file='closeye2.png')
        self.passw.config(show='•')
        self.eyebutton2.config(command=self.show)
    def show(self):
        self.eyebutton.config(file='openeye2.png')
        self.passw.config(show='')
        self.eyebutton2.config(command=self.hide)


    def on_username(self,event):
        if self.log.get()=="username":
            self.log.delete(0,END)

    def on_passw(self,event):
        if self.passw.get()=="password":
            self.passw.delete(0,END)
        
    


class Signup_Window:
    

    def __init__(self,root):
        self.root=root
        self.root.title("Signup")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        #self.root['bg']='#D2EFF2'



        bgimage=ImageTk.PhotoImage(file='tlo.jpg')
        self.bgimage=bgimage
        bgLabel=Label(self.root, image=bgimage)
        bgLabel.grid(row=0,column=0)

        frame=Frame(self.root, bg="white")
        frame.place(x=860,y=250, width=680, height=900)

        get_str=Label(frame, text="Create new account", font=("tahoma", 20, "bold"), fg="#3A949E", bg="white")
        get_str.place(x=75, y=90)

        user=Label(frame, text="Username", font=("tahoma", 10, "bold"), fg="#3A949E", bg="white")
        user.place(x=75, y=200)
        
        usernameEntry=Entry(self.root,  width=20, font=("tahoma",15), bg="lightblue", fg="#3A949E")
        usernameEntry.place(x=940, y=500)
        self.usernameEntry=usernameEntry

        passw=Label(frame, text="Password", font=("tahoma", 10, "bold"), fg="#3A949E", bg="white")
        passw.place(x=75, y=320)
        
        passwEntry=Entry(self.root,   width=20, font=("tahoma",15), bg="lightblue", fg="#3A949E")
        passwEntry.place(x=940, y=740)
        self.passwEntry=passwEntry

        passw2=Label(frame,  text="Confirm Password", font=("tahoma", 10, "bold"), fg="#3A949E", bg="white")
        passw2.place(x=75, y=440)
        
        passw2Entry=Entry(self.root,  width=20, font=("tahoma",15), bg="lightblue", fg="#3A949E")
        passw2Entry.place(x=940, y=620)
        self.passw2Entry=passw2Entry

        email=Label(frame,  text="E-mail",  font=("tahoma", 10, "bold"), fg="#3A949E", bg="white")
        email.place(x=75, y=560)
        
        emailEntry=Entry(self.root, width=20, font=("tahoma",15), bg="lightblue", fg="#3A949E")
        emailEntry.place(x=940, y=860)
        self.emailEntry=emailEntry



        
        goButton=Button(self.root, text='Sign up', font=('Open Sans', 15, 'bold'), fg='white', bg='#3A949E', bd=0, cursor='hand2', command=self.register_data)
        goButton.place(x=940,y=950)
        self.gobutton=goButton


        login=Button(self.root, text='Already have an acount? Log in',font=('Open Sans', 10), bg='white', fg='#9CBABF', bd=0, cursor='hand2', command=self.login_page)
        login.place(x=1100,y=1085)
        self.login=login
        
    def login_page(self):
        self.root.destroy()
        self.root=Tk()
        self.app=Login_Window(self.root)
    
    def register_data(self):
        regex = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,7}\b'
        conn=mysql.connector.connect(host="localhost", user="root", password="1234", database="mydb")
        my_cursor=conn.cursor()
        if self.usernameEntry.get()=="" or self.emailEntry.get()=="" or self.passwEntry.get()=="" or self.passw2Entry.get()=="":
            messagebox.showerror("Error", "All fields are required")
        elif self.passwEntry.get()!=self.passw2Entry.get():
            messagebox.showerror("Error", "Password and confirmed password should be the same")
        elif not(re.fullmatch(regex, self.emailEntry.get())):
            messagebox.showerror("Error", "Please enter a valid email address")
        else:
            query=("select * from users where email=%s")
            value=self.emailEntry.get()
            value=(value,)
            my_cursor.execute(query,value)
            row=my_cursor.fetchone()
            if row!=None:
                messagebox.showerror("Error", "User already exists, please try another email")
            else:
                query=("select * from users where username=%s")
                value=self.usernameEntry.get()
                value=(value,)
                my_cursor.execute(query,value)
                row=my_cursor.fetchone()
                hashp, salt,pepper =hash_password(self.passwEntry.get())
                #pepper=str(pepper)
                #user=str(self.usernameEntry.get())
                user=str(self.usernameEntry.get())
                if row==None:
                    save_pair_to_file("pepper.txt",user,pepper)
                    my_cursor.execute("insert into users(username,email,password,salt) values (%s,%s,%s, %s)",
                                    (
                                    self.usernameEntry.get(),
                                    self.emailEntry.get(),
                                    hashp,
                                    salt))
                    messagebox.showinfo("Success")
                else:
                    messagebox.showerror("Error", "User already exists, please try another username")
            conn.commit()
            conn.close()
            

class Start_Window():
    def __init__(self,root, userid):
        self.root=root
        self.user_id=userid
        self.root.title("Start")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        
       
                # Ścieżki do plików obrazków i odpowiadające im podpisy i funkcje obsługujące
        icons = [
            {"path": "ikon1.jpg", "label": "Whiteboard", "command": self.on_icon1_click},
            {"path": "ikon1.jpg", "label": "Library", "command": self.on_icon2_click},
            {"path": "ikon1.jpg", "label": "Przycisk 3", "command": self.on_icon3_click},
            {"path": "ikon1.jpg", "label": "Przycisk 4", "command": self.on_icon4_click},
            {"path": "ikon1.jpg", "label": "Przycisk 5", "command": self.on_icon5_click},
            {"path": "ikon1.jpg", "label": "Przycisk 6", "command": self.on_icon6_click},
        ]

        # Tworzenie ikon w dwóch rzędach
        for i, icon_data in enumerate(icons):
            row = i // 3  # Oblicz rzęd dla każdej ikony (0 lub 1)
            column = i % 3  # Oblicz kolumnę dla każdej ikony (od 0 do 2)
            self.create_icon(icon_data["path"], icon_data["label"], icon_data["command"], row, column)

        # Wyśrodkowanie ikon na stronie
        for i in range(3):  # Konfigurujemy trzy kolumny
            self.root.grid_columnconfigure(i, weight=1)

        # Konfigurujemy wysokość rzędów
        self.root.grid_rowconfigure(0, weight=1)
        self.root.grid_rowconfigure(1, weight=1)

    def create_icon(self, icon_path, label_text, command, row, column):
        icon_image = Image.open(icon_path)
        icon_image = self.resize_image(icon_image, size=(300, 300))  # Dostosuj rozmiar ikon
        icon_photo = ImageTk.PhotoImage(icon_image)

        # Kontener Frame dla każdego przycisku z etykietą
        icon_frame = Frame(self.root)
        icon_frame.grid(row=row, column=column, padx=100, pady=100)

        icon_button = Button(icon_frame, image=icon_photo, width=300, height=300, command=command)  # Ustaw jednolitą szerokość i wysokość
        icon_button.image = icon_photo  # Ta linijka jest potrzebna
        icon_button.pack()

        label = Label(icon_frame, text=label_text, font=("tahoma", 10, "bold"), fg="#3A949E", bg="white")
        label.pack(pady=30) #ustawia napis niżej pod obrazkiem

    def resize_image(self, image, size):
        return image.resize(size, Image.ANTIALIAS)

    def on_icon1_click(self):
        # Obsługa kliknięcia na ikonę 1
        print("Kliknięto na ikonę 1!")

    def on_icon2_click(self): ###################################################################################################
        # Obsługa kliknięcia na ikonę 2
        
        self.root.destroy()
        self.root=Tk()
        self.app=CE_Window(self.root, self.user_id)
        
        print("Kliknięto na ikonę 2!")

    def on_icon3_click(self):
        # Obsługa kliknięcia na ikonę 3
        print("Kliknięto na ikonę 3!")

    def on_icon4_click(self):
        # Obsługa kliknięcia na ikonę 4
        print("Kliknięto na ikonę 4!")

    def on_icon5_click(self):
        # Obsługa kliknięcia na ikonę 5
        print("Kliknięto na ikonę 5!")

    def on_icon6_click(self):
        # Obsługa kliknięcia na ikonę 6
        print("Kliknięto na ikonę 6!")






class CE_Window:
    def __init__(self, root,userid):
        self.root = root
        self.user_id=userid
        self.root.title("Computer Equipment Rental")
        self.root.geometry("2400x1400")
        root.resizable(0,0)
        background_image = Image.open("ce.jpg")
        background_image = background_image.resize((2400, 1400), Image.ANTIALIAS)
        self.bgimage = ImageTk.PhotoImage(background_image)

        # Utworzenie etykiety z obrazkiem tła
        bg_label = Label(self.root, image=self.bgimage)
        bg_label.place(x=0, y=0, relwidth=1, relheight=1)

        

        self.create_buttons()

    def create_buttons(self):
        # Tworzenie ramki do wyśrodkowania przycisków
        frame = Frame(self.root, bg='white')
        frame.pack(expand=True)

        # Styl napisów
        button_font = ('Rosewood Std Regular', 15, 'bold')

        button_available_equipment = Button(frame, text="add new coursebook", font=button_font, command=self.on_click_available_equipment, bg="#00a9be", bd=1)
        button_borrowing_history = Button(frame, text="my bookings and reservations", font=button_font, command=self.on_click_borrowing_history, bg="#00a9be", bd=1)
        button_borrow = Button(frame, text="borrow / make reservation", font=button_font, command=self.on_click_borrow, bg='#6aa84f', bd=1)
        button_return = Button(frame, text="return", font=button_font, command=self.on_click_return, bg='#d53131', bd=1)

        # Wyśrodkowanie przycisków na ekranie za pomocą pack
        button_available_equipment.pack(pady=10, padx=(10, 10), fill='both')
        button_borrowing_history.pack(pady=10, padx=(10, 10), fill='both')
        button_borrow.pack(pady=10, padx=(10, 10), fill='both')
        button_return.pack(pady=10, padx=(10, 10), fill='both')

    def on_click_available_equipment(self):
        self.root.destroy()
        self.root=Tk()
        self.app=Addnewbook(self.root, self.user_id)

    def on_click_borrowing_history(self):
        self.root.destroy()
        self.root=Tk()
        self.app=Mylibrary(self.root, self.user_id)

    def on_click_borrow(self):
        self.root.destroy()
        self.root=Tk()
        self.app=LiBorrow(self.root, self.user_id)

    def on_click_return(self):
        self.root.destroy()
        self.root=Tk()
        self.app=Mylibrary(self.root, self.user_id)
        
    
#-----------------------------------------------------------------------------------------------------------------------

def mail(email_receiver,type,title):
    email_sender='librarjany@gmail.com'

    email_password='chvbipxrnjzwyhxi'

    if(type==1):# borrowing
        subject='Library - borrowing'
        current_date=datetime.date.today()+datetime.timedelta(days=30)
        body="Book "+ title +" was succesfuly borrrowed. Due date is "+str(current_date)
    elif(type==2): #reserve
        subject='Library - reservation'
        current_date=datetime.date.today()+datetime.timedelta(days=7)
        body="Your reservation for book "+ title + " is successful. You can pick it up before " + str(current_date)
    elif(type==3): #add to wishlist
        subject='Library - wishlist'
        current_date=datetime.date.today()+datetime.timedelta(days=7)
        body="You were on the wishlist for "+ title +" book. Now it is reserved for you. You can pick it up before"+str(current_date)

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
        self.author_var = StringVar()
        author_combobox = Combobox(self.root, textvariable=self.author_var, values=authors)
        author_combobox.place(x=1420, y=40)
        author_combobox.bind("<<ComboboxSelected>>", self.on_author_selected)
        

        category = self.get_unique_category()
        self.category_var = StringVar()
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
            print("Database connection error", error)
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
                    title=cursor.fetchone()[0]

                    query='SELECT users.email FROM users where users.id=%s'
                    data=(str(self.user_id),)
                    cursor.execute(query, data)
                    receiver=cursor.fetchone()[0]

                    mail(receiver,1,title)
                    print("elo")
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
                    title=cursor.fetchone()[0]

                    query='SELECT users.email FROM users where users.id=%s'
                    data=(str(self.user_id),)
                    cursor.execute(query, data)
                    receiver=cursor.fetchone()[0]

                    mail(receiver,2,title)
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
            print("Database connection error", error)
        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()
            return k


#------------------------------------------------------------------------------------------------------------------------------------------------------------
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
            print("Database connection error", error)
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
                print("Database connection error", error)
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
                
            if(type==2): #return - dodaj date oddania do borrowing i ustaw return na 1, napisz do goscia na wishliscie, ze ksiazka sie zwolnila i zrob dla niego rezerwacje jesli moze on zarezerwowac
                query = "UPDATE borrowing SET borrowing.end=CURDATE(), borrowing.returned=1 WHERE borrowing.book_copy_id=%s;"
                data = (book_id,)
                cursor.execute(query, data)
                
                query='SELECT bookinfo.title FROM bookinfo where bookinfo.id=%s'
                data=(book_id,)
                cursor.execute(query, data)
                title=cursor.fetchone()[0]
               

                query='SELECT users.email, users.id FROM users JOIN wishlist ON wishlist.user_id=users.id where wishlist.book_copy_id=%s'
                data=(book_id,)
                cursor.execute(query, data)
                r=cursor.fetchone()
                if(r):
                    reciever=r[0]
                    reciever_id=r[1]


                    mail(reciever,3,title)

                    query="DELETE FROM WISHLIST WHERE  wishlist.book_copy_id=%s"
                    data=(book_id,)
                    cursor.execute(query, data)

                    query = "SELECT * from reserved WHERE user_id=%s and end>=NOW()"
                    data=(reciever_id,)
                    cursor.execute(query, data)
                    cursor.fetchall()
                    if(cursor.rowcount<5):
                        query = "INSERT INTO reservation(start, end, book_copy_id, user_id) values  (CURDATE(),CURDATE()+INTERVAL 7 DAY,%s,%s);"
                        data = (book_id, reciever_id)
                        cursor.execute(query, data)

                        query='SELECT bookinfo.title FROM bookinfo where bookinfo.id=%s'
                        data=(book_id,)
                        cursor.execute(query, data)
                        title=cursor.fetchone()[0]

                        query='SELECT users.email FROM users where users.id=%s'
                        data=(reciever_id,)
                        cursor.execute(query, data)
                        receiver=cursor.fetchone()[0]

                        mail(receiver,2,title)
                    else:
                        k=1
                        messagebox.showerror("Error", "You can only reserve 5 books in a while.")
                

                



            connection.commit()
            
        except mysql.connector.Error as error:
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
            print("Database connection error", error)
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
                print("Database connection error", error)
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

if __name__=="__main__":
    
    ctypes.windll.shcore.SetProcessDpiAwareness(1) #better quality of font
    root=Tk()
    app=Signup_Window(root)
    root.mainloop()
    
    

    '''
    haslo='jula'
    hashp,s,p=hash_password(haslo)
    print(verify_password(hashp,haslo,s,p))



   adminek, admin
   julia julia
'''
    









