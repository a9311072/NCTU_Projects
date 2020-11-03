import cv2
import numpy as np

def splitRGB(img):
    # TODO
    zero_map = np.zeros(img.shape[:2])
    R_map = np.stack((zero_map, zero_map, img[:,:,2]), axis =2)
    G_map = np.stack((zero_map, img[:,:,1], zero_map), axis =2)
    B_map = np.stack((img[:,:,0], zero_map,  zero_map), axis =2)
    return R_map, G_map, B_map

def splitHSV(img):
    # TODO
    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    H_map = img[:,:,0]
    S_map = img[:,:,1]
    V_map = img[:,:,2]
    return H_map, S_map, V_map

def resize(img, size):
    # TODO
    img_t = cv2.resize(img, (int(img.shape[1]*size), int(img.shape[0]*size)))
    return img_t

class MotionDetect(object):
    """docstring for MotionDetect"""
    def __init__(self, shape):
        super(MotionDetect, self).__init__()

        self.shape = shape
        self.avg_map = np.zeros((self.shape[0], self.shape[1], 3), dtype='float')
        self.alpha = 0.8 # you can ajust your value
        self.threshold = 40 # you can ajust your value

        print("MotionDetect init with shape {}".format(self.shape))

    def getMotion(self, img):
        assert img.shape == self.shape, "Input image shape must be {}, but get {}".format(self.shape, img.shape)

        # Extract motion part (hint: motion part mask = difference between image and avg > threshold)
        # TODO
        motion_part_mask = np.where(img - self.avg_map > self.threshold , 1, 0)
        
        # Mask out unmotion part (hint: set the unmotion part to 0 with mask)
        # TODO
        motion_map = img * motion_part_mask

        # Update avg_map
        # TODO
        self.avg_map = self.avg_map * self.alpha + img * (1-self.alpha)
        return motion_map


# ------------------ #
#     RGB & HSV      #
# ------------------ #
name = "../data.png"
img = cv2.imread(name)
if img is not None:
    print("Reading {} success. Image shape {}".format(name, img.shape))
else:
    print("Faild to read {}.".format(name))

R_map, G_map, B_map = splitRGB(img)
H_map, S_map, V_map = splitHSV(img)

cv2.imwrite('data_R.png', R_map)
cv2.imwrite('data_G.png', G_map)
cv2.imwrite('data_B.png', B_map)
cv2.imwrite('data_H.png', H_map)
cv2.imwrite('data_S.png', S_map)
cv2.imwrite('data_V.png', V_map)


# ------------------ #
#   Interpolation    #
# ------------------ #
name = "../data.png"
img = cv2.imread(name)
if img is not None:
    print("Reading {} success. Image shape {}".format(name, img.shape))
else:
    print("Faild to read {}.".format(name))

height, width, channel = img.shape
img_big = resize(img, 2)
img_small = resize(img, 0.5)
img_big_cv = cv2.resize(img, (width*2, height*2))
img_small_cv = cv2.resize(img, (width//2, height//2))

cv2.imwrite('data_2x.png', img_big)
cv2.imwrite('data_0.5x.png', img_small)
cv2.imwrite('data_2x_cv.png', img_big_cv)
cv2.imwrite('data_0.5x_cv.png', img_small_cv)

# ------------------ #
#  Video Read/Write  #
# ------------------ #
name = "../data.mp4"
# Input reader
cap = cv2.VideoCapture(name)
fps = cap.get(cv2.CAP_PROP_FPS)
h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))

# Output writer
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output1.avi', fourcc, fps, (w, h), True)

# Motion detector
mt = MotionDetect(shape=(h,w,3))

# Read video frame by frame
while True:
    # Get 1 frame
    success, frame = cap.read()

    if success:
        motion_map = mt.getMotion(frame)

        # Write 1 frame to output video
        out.write(np.uint8(motion_map))
    else:
        break

# Release resource
cap.release()
out.release()