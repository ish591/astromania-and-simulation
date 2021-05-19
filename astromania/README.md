# Astromania

Players move through the maze, blasting rocks, collecting power-ups to gain abilities and incinerating their opponents

The last man standing wins!

Description:
- Each player is equipped with 3 lives at the start
- Players can deploy bombs to destroy rocks and kill enemies
- Rocks yield collectible power-ups on destruction which provide users with special abilities
- After a certain time, the maze begins to close in a spiral fashion, making the game more dynamic and intense


For more details regarding the gameplay refer to [astromania.pdf](astromania.pdf)

## Client
The client can play an offline game or connect to the server to player online with other players. 

```bash
cd client
make # Builds the executable
make game # Runs the game
```

## Server
In order to run an online game, the clients need to connect to a server. To start the server:
```bash 
cd server
make # To build the server
make server # To run the server
make free # To free the port on which the server was hosted
```

In the real-world, this server would be hosted online

## Features

Apart from the basic functionality of movement of players and dropping bombs, collecting powerups and gaining abilities, the game has the following additional features:
- Fully functional menu
- Online Game Waiting Room: A waiting room for players while other users join
- Smooth movement - Analysed on pixel level for continuity
- Timed and unlimited abilities - faster movement
- Directional rendering of textures for characters
- Dead body rendering at place of death
- Players start with 3 lives which are displayed dynamically throughout the game
- Blinking and immunity for 4s on loss of life
- After a certain time, the maze begins to close making the game more - dynamic and intense
- Accepts keyboard as well as mouse input
- Music: Specialised textures and soundtracks for enhanced experience.
- An option to mute/unmute the music
- Sounds: Explosions and Game ending
- Ending screen which showcases the winner
- Fully functional sockets and timed packets for a 0-lag experience
