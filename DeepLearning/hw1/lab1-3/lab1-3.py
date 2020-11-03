import cv2
import numpy as np

def avgFilter(img):
    # TODO
    img_avg = cv2.blur(img,(3,3))
    return img_avg

def midFilter(img):
    # TODO
    img_mid = cv2.medianBlur(img,5)
    return img_mid

def edgeSharpen(img):
    # TODO
    kernel = np.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]])
    img_edge = cv2.filter2D(img, -1, kernel)

    kernel = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
    img_s = cv2.filter2D(img_edge, -1, kernel)
    return img_edge, img_s

# ------------------ #
#       Denoise      #
# ------------------ #
name1 = '..\\noise_impulse.png'
name2 = '..\\noise_gauss.png'
noise_imp = cv2.imread(name1, 0)
noise_gau = cv2.imread(name2, 0)

img_imp_avg = avgFilter(noise_imp)
img_imp_mid = midFilter(noise_imp)
img_gau_avg = avgFilter(noise_gau)
img_gau_mid = midFilter(noise_gau)

cv2.imwrite('img_imp_avg.png', img_imp_avg)
cv2.imwrite('img_imp_mid.png', img_imp_mid)
cv2.imwrite('img_gau_avg.png', img_gau_avg)
cv2.imwrite('img_gau_mid.png', img_gau_mid)


# ------------------ #
#       Sharpen      #
# ------------------ #
name = '..\\mj.tif'
img = cv2.imread(name, 0)

img_edge, img_s = edgeSharpen(img)
cv2.imwrite('mj_edge.png', img_edge)
cv2.imwrite('mj_sharpen.png', img_s)