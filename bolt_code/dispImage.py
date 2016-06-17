from tkinter import *
import datetime as dt

file_part = '/home/brainwave/glstl/png/slice_'

global counter
counter = 0
extension = '.png'

def stuff(event):
    global picture3 
    global counter

    counter = counter+1
    picture3 = PhotoImage(file=file_part+str(counter)+extension)
    c.itemconfigure(picture2, image = picture3)

def stop(event):
    global picture3
    
    picture3 = PhotoImage(file='/home/brainwave/glstl/blank.png')
    c.itemconfigure(picture2, image=picture3)

    	
main = Tk()
main.attributes("-fullscreen", True)
c = Canvas(main, width=1200, height=800)
c.pack()

picture = PhotoImage(file=file_part+str(counter)+extension)
picture2 = c.create_image(600,400,anchor=CENTER, image=picture)

Button(main, text='Quit', command=main.destroy).pack()
main.bind("<Return>",stuff)
main.bind("o",stop)

main.mainloop()
