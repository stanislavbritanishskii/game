from PIL import Image
import os

# Get all PNG files in the current directory
png_files = [f for f in os.listdir() if f.lower().endswith('.png')]

# Process each PNG file
for file in png_files:
    # Open the image
    image = Image.open(file)
    
    # Get image dimensions
    width, height = image.size
    
    # Define the crop area (16 pixels from each side)
    crop_area = (16, 8, width - 16, height - 24)
    
    # Crop the image
    cropped_image = image.crop(crop_area)
    
    # Save the cropped image
    output_file = f"{file}"
    cropped_image.save(output_file)
    
    print(f"Processed and saved: {output_file}")

print("All PNG files have been processed.")
