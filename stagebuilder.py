#!/usr/bin/env python

import argparse

from PIL import Image

# Green value of pixel at grid with X offset 1
BRICK = 7
STEEL = 174
FOREST = 79
WATER = 64
ICE = 102  # no offset at X


def main(file):
    print("File: ", file, "\n")
    forest = "Forest: \n{\n"
    water = "Water: \n{\n"
    ice = "Ice: \n{\n"
    walls = "Walls: \n{\n"
    for i in range(36):
        forest += f"// Stage {i} {{{{{{2\n{{\n"
        water += f"// Stage {i} {{{{{{2\n{{\n"
        ice += f"// Stage {i} {{{{{{2\n{{\n"
        walls += f"// Stage {i} {{{{{{2\n{{\n"
        next_file = f"{file.split('_')[0]}_{str(i).zfill(2)}.png"
        try:
            with Image.open(next_file) as img:
                img = img.convert("RGBA")
                for y in range(13):
                    for x in range(13):
                        # Skip base
                        if x > 4 and x < 8 and y > 10:
                            continue

                        _, g, _, _ = img.getpixel((x * 16 + 1, y * 16))  # type:ignore
                        if g == FOREST:
                            forest += f"{y * 100 + x}, \n"
                        elif g == WATER:
                            water += f"{y * 100 + x}, \n"
                        elif g == STEEL:
                            _, g, _, _ = img.getpixel((x * 16, y * 16))  # type:ignore
                            if g == ICE:
                                ice += f"{y * 100 + x}, \n"
                            else:
                                # top left is steel
                                _, g, _, _ = img.getpixel((x * 16 + 8, y * 16))  # type:ignore
                                if g == STEEL:
                                    # top right is steel
                                    _, g, _, _ = img.getpixel((x * 16, y * 16 + 8))  # type:ignore
                                    if g == STEEL:
                                        # bottom left is steel
                                        walls += f"{{{100 * y + x}, 7, 0}}, \n"
                                    else:
                                        # bottom left is empty
                                        walls += f"{{{100 * y + x}, 5, 0}}, \n"
                                else:
                                    # top right is empty
                                    _, g, _, _ = img.getpixel((x * 16, y * 16 + 8))  # type:ignore
                                    if g == STEEL:
                                        # bottom left is steel
                                        walls += f"{{{100 * y + x}, 5, 3}}, \n"

                        elif g == BRICK:
                            _, g, _, _ = img.getpixel((x * 16 + 8, y * 16))  # type:ignore
                            if g == BRICK:
                                # top right is brick
                                _, g, _, _ = img.getpixel((x * 16, y * 16 + 8))  # type:ignore
                                if g == BRICK:
                                    # bottom left is brick
                                    walls += f"{{{100 * y + x}, 3, 0}}, \n"
                                else:
                                    # bottom left is empty
                                    walls += f"{{{100 * y + x}, 1, 0}}, \n"
                            else:
                                # top right is empty
                                _, g, _, _ = img.getpixel((x * 16, y * 16 + 8))  # type:ignore
                                if g == BRICK:
                                    # bottom left is brick
                                    walls += f"{{{100 * y + x}, 1, 3}}, \n"
                        else:
                            _, g, _, _ = img.getpixel((x * 16, y * 16 + 8))  # type:ignore
                            if g == STEEL:
                                # bottom left is steel
                                walls += f"{{{100 * y + x}, 5, 2}}, \n"
                            elif g == BRICK:
                                # bottom left is brick
                                walls += f"{{{100 * y + x}, 1, 2}}, \n"
                            else:
                                _, g, _, _ = img.getpixel((x * 16 + 8, y * 16))  # type:ignore
                                if g == STEEL:
                                    # top right is steel
                                    walls += f"{{{100 * y + x}, 5, 1}}, \n"
                                elif g == BRICK:
                                    # top right is brick
                                    walls += f"{{{100 * y + x}, 1, 1}}, \n"

            forest += "},\n\n"
            water += "},\n\n"
            ice += "},\n\n"
            walls += "},\n\n"

        except FileNotFoundError:
            break

    forest += "}\n\n"
    water += "}\n\n"
    ice += "}\n\n"
    walls += "}\n\n"
    print(forest)
    print(water)
    print(ice)
    print(walls)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("file", type=str, help="Path to image file")
    args = parser.parse_args()
    main(file=args.file)
