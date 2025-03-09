![image](https://github.com/user-attachments/assets/3b9bb990-1c28-4b5b-9b7e-40df5f482070)

# so_long
A 2D game built using the MiniLibX graphics library. The project involved parsing a map file, handling player movement, rendering sprites, and implementing basic game mechanics. 

## 📖 Concepts covered in so_long
- Loading displaying, and animating sprites
- Handling frame updates and screen refreshing
- Event Handling & User Input
- Map Parsing & Validation


## ⬇️ Clone
```
git clone git@github.com:Sheldon-Chong/so_long.git
```

## ⏯️ Compile and run
```
make
./so_long <map name>
```

## ❓ How to play?
- Run the program using one of the available maps (or create your own)
- Use `W` `A` `S` `D` keys to navigate the player
- The player must first pass through all collectibles on the map. Doing so will allow them to access the exit
- The player must avoid the sentries. Sentries have a range and line of sight, and if the player is detected within that range, they will glide towards the player
- Press `Esc` to quit the game at any time

## My experience with this project
Typically, so_long is done in 2D, top-down fashion. 

However, I had the ambitious idea of attempting to make my project a real-time, side-scrolling, stealth-based, isometrically-rendered game.
I strongly recommend checking out this article I wrote, detailing the complexities of developing this game <a href="https://sheldon-chong.notion.site/so_long-aa58b4f143ef4a76997f9ec1671dc37f?pvs=4">developing so_long - Sheldon</a>

Having had experience developing isometric-graphics in the past, as well as animation and sprite-design, I got to work on this project. 
Below are the aspects that I used as the blueprint definition for this game:
- “**stealth-based**”: I had to design enemy AI in a way that would allow the player to remain undetected under certain conditions. This implied that I had to include raytracing to detect the player's position using a line of sight.
- “**side-scrolling**”: My game has to have a camera object that will move dynamically alongside the player. The concept of a camera is common in other game engines like Unity or Unreal.
- “**isometric**”: Meant that assets had to be designed with a diagonal shape, that connects seamless, and be mapped diagonally.

