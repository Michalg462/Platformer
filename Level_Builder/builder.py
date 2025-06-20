import tkinter as tk
from PIL import Image, ImageTk
from tkinter import filedialog

# Basic configuration
TILE_SIZE = 16      # Size of a tile in the original tileset (in pixels)
BG_SIZE = 64        # Size of a background tile in original (pixels)
SCALE = 2           # Visual scale factor for display
GRID_WIDTH = 12     # Number of tiles horizontally in the grid
GRID_HEIGHT = 12    # Number of tiles vertically in the grid
TILESET_PATH = "terrain.bmp"  # Path to tileset image
BACKGROUNDS_PATH = "backgrounds.bmp" # Path to a backgrounds image

class LevelEditor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Level Builder")

        # Loads the tileset image
        self.tileset = Image.open(TILESET_PATH)
        self.backgrounds = Image.open(BACKGROUNDS_PATH)
        self.tile_images = []       # List of loaded and resized tile images
        self.selected_tile = 0      # Currently selected tile index
        self.bg_images = []         # List of loaded backgrounds
        self.selected_background = 0    # Currently selected background
        self.grid_data = [[-1] * GRID_WIDTH for _ in range(GRID_HEIGHT)]  # -1 means empty cell



        # Canvas for the level grid
        self.canvas = tk.Canvas(
            self,
            width=GRID_WIDTH * TILE_SIZE * SCALE,
            height=GRID_HEIGHT * TILE_SIZE * SCALE,
            bg="light grey"
        )
        self.canvas.pack(side="left")
        self.canvas.bind("<Button-1>", self.on_click)  # Left-click to place tile

        # Frame to hold the tile palette and buttons
        self.palette = tk.Frame(self)
        self.palette.pack(side="right", fill="y")

        self.load_tiles()      # Load and display palette buttons
        self.add_bg_selector() # Load and add backgrounds to select
        self.add_controls()    # Add "Clear" and "Save" buttons
        self.draw_grid()       # Draw the initial (empty) level grid

    def load_tiles(self):
        # Split tileset into individual tiles and create buttons
        sheet_width = self.tileset.width // TILE_SIZE
        sheet_height = self.tileset.height // TILE_SIZE

        for row in range(sheet_height):
            for col in range(sheet_width):
                # Extract a tile from the tileset
                tile = self.tileset.crop((
                    col * TILE_SIZE, row * TILE_SIZE,
                    (col + 1) * TILE_SIZE, (row + 1) * TILE_SIZE
                ))
                # Resize tile for display (scaled)
                resized = tile.resize((TILE_SIZE * SCALE, TILE_SIZE * SCALE), Image.NEAREST)
                tk_img = ImageTk.PhotoImage(resized)
                self.tile_images.append(tk_img)

                # Create a button for this tile
                btn = tk.Button(
                    self.palette,
                    image=tk_img,
                    command=lambda i=len(self.tile_images)-1: self.select_tile(i)
                )
                btn.grid(row=row, column=col)

    def add_bg_selector(self):
        bg_width = self.backgrounds.width // BG_SIZE    # width of a background file
        for bg_num in range (bg_width):
            # get all backgrounds from file
            tile = self.backgrounds.crop((
                bg_num * BG_SIZE, 0,
                (bg_num + 1) * BG_SIZE, BG_SIZE
            ))

            # Resize loaded parts, then add them as buttons to the grid
            resized = tile.resize((TILE_SIZE * SCALE, TILE_SIZE * SCALE), Image.NEAREST)
            tk_img = ImageTk.PhotoImage(resized)
            self.bg_images.append(tk_img)

            btn = tk.Button(
                self.palette,
                image=tk_img,
                command=lambda i=len(self.bg_images)-1: self.select_bg(i)
            )
            btn.grid(row=20, column=bg_num)

    def add_controls(self):
        # "Clear" button resets the grid
        clear_button = tk.Button(self.palette, text="Clear", command=self.clear_grid)
        clear_button.grid(row=21, column=0, columnspan=5, pady=5)

        # "Save" button exports level to file
        save_button = tk.Button(self.palette, text="Save", command=self.save_grid)
        save_button.grid(row=21, column=1, columnspan=5, pady=5)

    def select_tile(self, idx):
        # Set the currently selected tile index
        self.selected_tile = idx

    def select_bg(self, idx):
        # Set currently selected bg index
        self.selected_background = idx

    def clear_grid(self):
        # Reset all tiles to empty
        self.grid_data = [[-1] * GRID_WIDTH for _ in range(GRID_HEIGHT)]
        self.draw_grid()

    def save_grid(self):
        # Ask the user where to save the level file
        tiles_per_row = self.tileset.width // TILE_SIZE
        filepath = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text files", "*.txt")],
            title="Save Level As"
        )
        if not filepath:
            return

        # Write background and non-empty tile data to file
        with open(filepath, "w") as file:
            file.write(f"{self.selected_background}\n")
            for y in range(GRID_HEIGHT):
                for x in range(GRID_WIDTH):
                    tile = self.grid_data[y][x]
                    if tile != -1:
                        pixel_x = x * TILE_SIZE
                        pixel_y = y * TILE_SIZE
                        type_number = tile
                        file.write(f"{pixel_x} {pixel_y} {type_number}\n")

    def on_click(self, event):
        # Convert mouse click to grid coordinates and place selected tile
        x = event.x // (TILE_SIZE * SCALE)
        y = event.y // (TILE_SIZE * SCALE)
        if 0 <= x < GRID_WIDTH and 0 <= y < GRID_HEIGHT:
            self.grid_data[y][x] = self.selected_tile
            self.draw_grid()

    def draw_grid(self):
        # Clear and redraw the level grid, including tile images and outlines
        self.canvas.delete("all")
        for y in range(GRID_HEIGHT):
            for x in range(GRID_WIDTH):
                px = x * TILE_SIZE * SCALE
                py = y * TILE_SIZE * SCALE

                if self.grid_data[y][x] == -1:
                    # Draw a light grey square for empty tiles
                    self.canvas.create_rectangle(
                        px, py, px + TILE_SIZE * SCALE, py + TILE_SIZE * SCALE,
                        fill="light grey", outline=""
                    )
                else:
                    # Draw the tile image
                    tile = self.grid_data[y][x]
                    self.canvas.create_image(px, py, image=self.tile_images[tile], anchor="nw")

                # Draw a black border (gridline)
                self.canvas.create_rectangle(
                    px, py, px + TILE_SIZE * SCALE, py + TILE_SIZE * SCALE,
                    outline="black"
                )

if __name__ == "__main__":
    editor = LevelEditor()
    editor.mainloop()
