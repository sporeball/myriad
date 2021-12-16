# myriad

**myriad** is a game of packing problems -- several thousand of them, to be exact.

### install
you can compile the game using [gcc](https://gcc.gnu.org):

```
gcc -Wall -lncurses myriad.c -o myriad -lm
```

alternatively, check the [releases](https://github.com/sporeball/myriad/releases) section for a built executable.

the comment at the top of the source file describes how to play.

### about
myriad is based on the game [Quadrillion](https://www.smartgames.eu/uk/one-player-games/quadrillion), in which you snap four 4x4 grids together to create a unique game board, and must fit 12 colored pieces on it without covering the marked dots. it was originally written as a submission to the 9th round of the [Esolangs Discord server](https://discord.gg/3UXSK5p)'s *code guessing* event, where i placed 2nd after **none** of the 8 other participants was able to correctly guess that i had written it.

there are a few things which myriad omits from the original game:
- Quadrillion allows the grids to be connected in any configuration which forms one of 11 different board shapes; myriad implements just one of these shapes (a square)
- Quadrillion allows each of the grids to be placed with either side up, to increase the total number of possibilities; myriad implements only the dark side of each grid

even with these omissions, myriad is able to produce 6,144 of Quadrillion's many possible puzzles -- and all of them are guaranteed to have at least one solution.

the ["original"](https://github.com/sporeball/myriad/releases/original) release is the final version submitted to the original event, which was written over roughly two weeks and was not source controlled; any further releases will include bug fixes and improvements to this version.

### donate
you can support the development of this project and others via Patreon:

[![Support me on Patreon](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dsporeball%26type%3Dpledges%26suffix%3D%252Fmonth&style=for-the-badge)](https://patreon.com/sporeball)
