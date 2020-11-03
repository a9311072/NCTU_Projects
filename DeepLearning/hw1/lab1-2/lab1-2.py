import cv2
import numpy as np
import matplotlib.pyplot as plt

def gammaCorrection(img, gamma=1.0):
    # Build a lookup table mapping the pixel values [0, 255] to their adjusted gamma values
    # TODO
    img_g = img.copy( )
    nr, nc = img.shape[:2]
    c = 255.0 / ( 255.0 ** gamma )
    table = np.zeros( 256 )
    for i in range( 256 ):
        table[i] = round( i ** gamma * c, 0 )

    # Apply gamma correction using the lookup table
    # TODO
    if img.ndim != 3:
        for x in range( nr ):
            for y in range( nc ):
                img_g[x,y] = table[img[x,y]]
    else:
        for x in range( nr ):
            for y in range( nc ):
                for k in range( 3 ):
                    img_g[x,y,k] = table[img[x,y,k]]

    return img_g

def histEq(gray):
    # Calculate histogram
    hist = cv2.calcHist([gray], [0], None, [256], [0, 256]).reshape(-1)
    hist = hist / gray.size
    print(hist)

    # Convert the histogram to Cumulative Distribution Function
    # TODO
    table = np.zeros(256)
    for i in range(256):
        table[i] = hist[:i+1].sum()
    # Build a lookup table mapping the pixel values [0, 255] to their new grayscale value
    # TODO
    m = table.min()
    for i in range(256):
        table[i] = round((table[i] - m) * 255)

    # Apply histogram equalization using the lookup table
    # TODO
    img_h = gray.copy( )
    nr, nc = gray.shape[:2]
    if gray.ndim != 3:
        for x in range( nr ):
            for y in range( nc ):
                img_h[x,y] = table[gray[x,y]]
    else:
        for x in range( nr ):
            for y in range( nc ):
                for k in range( 3 ):
                    img_h[x,y,k] = table[gray[x,y,k]]

    return img_h


# ------------------ #
#  Gamma Correction  #
# ------------------ #
name = "../data.mp4"
cap = cv2.VideoCapture(name)
success, frame = cap.read()
if success:
    print("Success reading 1 frame from {}".format(name))
else:
    print("Faild to read 1 frame from {}".format(name))
cap.release()

gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
img_g1 = gammaCorrection(gray, 0.5)
img_g2 = gammaCorrection(gray, 2)
cv2.imwrite('gray.png', gray)
cv2.imwrite('data_g0.5.png', img_g1)
cv2.imwrite('data_g2.png', img_g2)

# ------------------------ #
#  Histogram Equalization  #
# ------------------------ #
name = "../hist.png"
img = cv2.imread(name, 0)

img_h = histEq(img)
img_h_cv = cv2.equalizeHist(img)
cv2.imwrite("hist_h.png", img_h)
cv2.imwrite("hist_h_cv.png", img_h_cv)

# save histogram
plt.figure(figsize=(18, 6))
plt.subplot(1,3,1)
plt.bar(range(1,257), cv2.calcHist([img], [0], None, [256], [0, 256]).reshape(-1))
plt.subplot(1,3,2)
plt.bar(range(1,257), cv2.calcHist([img_h], [0], None, [256], [0, 256]).reshape(-1))
plt.subplot(1,3,3)
plt.bar(range(1,257), cv2.calcHist([img_h_cv], [0], None, [256], [0, 256]).reshape(-1))
plt.savefig('hist_plot.png')