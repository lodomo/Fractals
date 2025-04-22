from PIL import Image

def png_to_font(input_path, output_path):
    img = Image.open(input_path).convert("1")  # Convert to 1-bit B&W
    width, height = img.size
    pixels = img.load()

    with open(output_path, "w") as f:
        for y in range(height):
            row_bits = "{"
            for x in range(width):
                row_bits += "0" if pixels[x, y] == 255 else "1"
                row_bits += ", "
            f.write(row_bits + "},\n")

    print(f"Saved {width}x{height} bitmap to {output_path}")

# Example usage
png_to_font("font.png", "output.font")

