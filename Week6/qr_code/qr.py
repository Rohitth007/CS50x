# Generated a QR Code
# https://github.com/lincolnloop/python-qrcode
import qrcode

img = qrcode.make("Rohitth the Great.")
img.save("qr.jpg")
