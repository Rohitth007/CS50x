# Searches for Obama's picture in other pics
# https://github.com/ageitgey/face_recognition/blob/master/examples/identify_and_draw_boxes_on_faces.py#L54
import face_recognition
import numpy as np
from PIL import Image, ImageDraw


#load image
obama_image = face_recognition.load_image_file("known_pics/obama.jpg")
obama_face_encoding = face_recognition.face_encodings(obama_image)
# ***returns an array*** with 128-dimention face encoding for each face in the image.
# In this case the array will have only one element as there is only one face.

#load unknown image
unknown_image = face_recognition.load_image_file("unknown_pics/two_people.jpg")
face_locations = face_recognition.face_locations(unknown_image)
face_encodings = face_recognition.face_encodings(unknown_image, face_locations)
# returns an encoded array with the same order as the face_locations array

# Convert image to PIL-format so that we can draw on it
pil_image = Image.fromarray(unknown_image)
# Create Pillow ImageDraw Draw instance to draw with
draw = ImageDraw.Draw(pil_image)

# Loop through each face found in the unknown_image and check if it matches
# with the encoding of the known_face
for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
    matches = face_recognition.compare_faces(obama_face_encoding, face_encoding)
    # Returns a list of True/False for each face in the ***list 'obama_face_encoding'***.
    # Here list size is 1.

    name = "Unknown"
    if matches[0]:
        name = "Obama"

    # Draw box
    draw.rectangle(((left, top), (right, bottom)), outline = (255, 255, 0))
    # Draw label
    text_width, text_height = draw.textsize(name)
    draw.rectangle(((left, bottom - text_height - 10), (right, bottom)), fill = (255, 255, 0), outline = (255, 255, 0))
    draw.text((left + 6, bottom - text_height - 5), name, fill = (0, 0, 0, 255))

# Remove the drawing library from memory
del draw

pil_image.show()
pil_image.save("face_boxed.jpg")
