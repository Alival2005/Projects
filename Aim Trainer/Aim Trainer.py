import tkinter
import random

def targetClickHandler():
    global score
    rand_x_coord = random.randint(0, 1240)
    rand_y_coord = random.randint(0, 650)
    button.place(x=rand_x_coord, y=rand_y_coord)
    score += 1
    score_button.configure(text=f"Score = {score}")

def start():
    global score
    global Hscore
    if int(score) > int(Hscore):
        Hscore = score
        high_score_label.configure(text=f"High Score = {Hscore}")
        with open("HighScore.txt", "w") as file_2:
            file_2.write(f"{score}")
    score = 0
    score_button.configure(text=f"Score = {score}")
    rand_x_coord = random.randint(0, 1240)
    rand_y_coord = random.randint(0, 650)
    button.place(x=rand_x_coord,y=rand_y_coord)
    
score=0
window = tkinter.Tk()
window.title("Aim Trainer")
window.geometry("1280x720")
window.configure(bg="Black")
window.resizable(False,False)

button = tkinter.Button(window, width=5, height=2, font="50", command=targetClickHandler)
start_button=tkinter.Button(text="Start", command=start)
start_button.place(x=1220,y=696)

score_button=tkinter.Label(text="Score = 0", fg="White", bg="Black", font=("Arial", 15, "bold"))
score_button.place(x=515, y=20)

with open("HighScore.txt", "r") as file:
    Hscore = file.read()

high_score_label=tkinter.Label(text=f"High Score = {Hscore}", fg="White", bg="Black", font=("Arial", 15, "bold"))
high_score_label.place(x=680, y=20)

exit_button = tkinter.Button(text="Exit",command=lambda: window.quit())
exit_button.place(x=1250,y=696)

window.mainloop()

if int(score) > int(Hscore):
    Hscore = score
    with open("HighScore.txt", "w") as file_2:
        file_2.write(f"{score}")