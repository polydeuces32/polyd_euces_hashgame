# POLYD_EUCES_HASHGAME

A C++ terminal puzzle game prototype built around Bitcoin vocabulary and proof-of-work concepts.

The player receives a clue, chooses from five possible answers, and earns points when the answer matches.

## Current status

This repo now includes a working C++ program:

- `polyd_euces_hashgame.cpp` — playable terminal game
- `BUILD.md` — compile and run instructions
- `NOTES.md` — project notes

## Build

See `BUILD.md` for the full command.

Quick compile:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 polyd_euces_hashgame.cpp -o polyd_euces_hashgame
```

Run:

```bash
./polyd_euces_hashgame
```

## Roadmap

- Add stronger puzzle rounds
- Add difficulty levels
- Add score history
- Add a cleaner menu system
- Add real hash verification later
- Add Signet-style payout simulation later

## License

MIT License.
