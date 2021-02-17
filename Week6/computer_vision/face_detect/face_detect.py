from PIL import Image
import face_recognition
import time

# load image into numpy array.
image = face_recognition.load_image_file("pics/cinema.jpeg")

# Find all the faces in the image using the default HOG-based model. (Histogram of Oriented Gradients)
# This method is fairly accurate, but not as accurate as the CNN model and not GPU accelerated.
face_locations = face_recognition.face_locations(image)
# face_locations is now a array listing all the coordinates of each face.

print(f"I have found {len(face_locations)} faces in this image.")

i = 0
for face_location in face_locations:

    top, right, bottom, left = face_location
    print(f"A face is located at pixel location Top: {top}, Left: {left}, Bottom: {bottom}, Right: {right}")

    # Accessing each face, equivalent to cropping:
    face_image = image[top:bottom, left:right]
    pil_image = Image.fromarray(face_image)
    pil_image.save('pics/cinema_faces/' + str(i) + '.jpg')
    i += 1
    #time.sleep(3)
