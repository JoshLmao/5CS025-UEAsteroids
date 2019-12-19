<p align="center">
  <h1>Hyperoids</h1>
  <a href="https://github.com/JoshLmao/5CS025-UEAsteroids">
    <img src="https://img.shields.io/badge/github-UE Asteroids-black.svg?style=flat-square.svg"/>
  </a>
  <a href="https://twitter.com/JoshLmao">
    <img src="https://img.shields.io/badge/twitter-JoshLmao-blue.svg?style=flat-square.svg"/>
  </a>
</p>

## About

A remake of the classic Asteroids game inside Unreal Engine 4 (4.22.3). The player must survive in space against endless spawning asteroids and alien ships that fire at the player.

## Game Setup Requirements

When creating a level using the 'HyperoidsGameModeBase', a spawner of type 'AsteroidSpawner' is required which will layout the total play area for the player to fly around in. It is required since the 'BasicAsteroid' 'AlienShip' and 'SpaceshipPawn' classes need a play area FVector2D to wrap around and continue play.

## Main Camera

A camera of type 'TopDownCamera' can be used to set the main view target of the game. Place the camera inside the world above the target play area

## Main Player

The player, contained inside 'SpaceshipPawn', can fly around the play area specified, while wrapping around from one side of the screen to the other. They can fly up, down, left and right using W, A, S and D and can also fire at asteroids and alien ships using spacebar

## Spawners

New spawners can be created inheriting from the 'ActorSpawner' class which allows you to specify an area in which the actors will spawn, while also specifying how many and how fast they will spawn.

The 'AsteroidSpawner' lays out the total play area of the game while also spawning in actors (of type 'BasicAsteroid', although there isn't a restriction). A safe area can be specified using the 'SafeArea' property which force asteroids so spawn outside those coordinates

'AlienShipSpawner' can also spawn in actors (of type 'AlienShip', although no restriction) and specify an area where the actors will spawn inside. It can also specify a range to use for the actors movement direction