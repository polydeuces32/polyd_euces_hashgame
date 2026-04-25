# POLYD_EUCES_HASHGAME

A C++ terminal puzzle game prototype built around Bitcoin vocabulary, puzzle verification, and simulated rewards.

The player receives a clue, sees five answer choices, and earns simulated credits when the chosen answer matches the verifier fingerprint.

## Current status

The `feature/phases-1-4` branch upgrades the project with:

- Phase 1: verifier function for answer fingerprints
- Phase 2: verifier connected to puzzle answers
- Phase 3: simulated reward scoring
- Phase 4: round history summary

## Files

- `polyd_euces_hashgame.cpp` — playable C++ terminal game
- `BUILD.md` — compile and run instructions
- `NOTES.md` — project notes

## Build

Compile with a C++17 compiler:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 polyd_euces_hashgame.cpp -o polyd_euces_hashgame
```

Run:

```bash
./polyd_euces_hashgame
```

## Gameplay

- Read the clue
- Choose one of five options or type the answer manually
- The game verifies your answer using a generated fingerprint
- Correct answers increase your simulated credit score
- When you quit, the game prints your round history

## Safety note

This is an educational game prototype. Rewards are simulated credits only. No real wallet, private key, transaction, or payout logic is included.

## Next roadmap

- Split verifier logic into separate files
- Add unit tests
- Add difficulty modes
- Add persistent local score history
- Add optional Signet documentation later without automatic wallet actions

## License

MIT License.
