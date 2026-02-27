# VoltorbFlip DS

A homebrew implementation for the **Nintendo DS** of the namesake minigame appeared in *Pokemon HeartGold and SoulSilver*.

Turn the right cards to gain points. Careful not to reveal the bombs!

## For Players

To play the game, save the file `VoltorbFlip.nds` (you can find it in the releases) on your flashcard (e.g., R4), modded console (DS/3DS), or load it into a Nintendo DS emulator (e.g. [**melonDS**](https://melonds.kuribo64.net/)).

### Quick Controls

* **D-Pad**: Move the cursor.
* **Tasto A**: Turn the selected card.
* **Tasto B**: Toggle the markers.
* **Touch Screen**: Directly interact with the boards and the markers.

## For the developers

This project uses the toolchain **BlocksDS** and the **NF_Lib** library.

### Prerequisites 
1. Make sure to have [`BlocksDS`](https://blocksds.skylyrac.net/docs/) installed and configured in the system path. 
2. Ensure that the dependancies are correctly installed: `libnds`, `Maxmod9`.
3. Make sure to have [`NF_lib`](https://github.com/knightfox75/nds_nflib) for BlocksDS

### Compilation

To build the `.nds` ROM from source:

1. Open your terminal in the project root directory.
2. Run the following command:
```bash
make

```

3. The compiler will generate `VoltorbFlip.nds` using the assets found in `/nitrofiles`.

## Technical Details

* **Graphics Engine**: Powered by **NF_Lib** for advanced sprite management and background handling.
* **Audio System**: Managed via **Maxmod**, supporting `.xm` and `.mod` formats for music and `.wav` for sound effects.
* **Game Logic**: Features a dynamic difficulty system that scales across **9 levels** based on your performance.
* **State Management**: Uses a centralized `SharedGameState` with dirty flags to optimize rendering and input processing.