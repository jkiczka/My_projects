from tkinter import*

import ctypes
from tkinter import ttk
from PIL import Image, ImageTk



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

        
        loginButton=Button(self.root, text='login', font=('Open Sans', 20, 'bold'), fg='white', bg='#3A949E', bd=0, cursor='hand2')
        loginButton.place(x=1110,y=880)
        self.loginbutton=loginButton


        register=Button(self.root, text='Create new account',font=('Open Sans', 10), bg='white', fg='#9CBABF', bd=0, cursor='hand2')
        register.place(x=1255,y=1085)
        self.registerbutton=register
        
  
        
    
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
        
    





if __name__=="__main__":
    ctypes.windll.shcore.SetProcessDpiAwareness(1) #better quality of font
    root=Tk()
    app=Login_Window(root)
    root.mainloop()