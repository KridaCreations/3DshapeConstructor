# Import required modules
import cv2
import numpy as np
import os
import glob


# Define the dimensions of checkerboard
CHECKERBOARD = (7, 7)
SQUARE_SIZE = 22


# stop the iteration when specified
# accuracy, epsilon, is reached or
# specified number of iterations are completed.
criteria = (cv2.TERM_CRITERIA_EPS +
            cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)


# Vector for 3D points
threedpoints = []

# Vector for 2D points
twodpoints = []


#  3D points real world coordinates
objectp3d = np.zeros((1, CHECKERBOARD[0]
                      * CHECKERBOARD[1],
                      3), np.float32)
objectp3d[0, :, :2] = np.mgrid[0:CHECKERBOARD[0],
0:CHECKERBOARD[1]].T.reshape(-1, 2) * SQUARE_SIZE
prev_img_shape = None


# Extracting path of individual image stored
# in a given directory. Since no path is
# specified, it will take current directory
# jpg files alone
script_dir = os.path.dirname(os.path.abspath(__file__))
images = glob.glob(os.path.join(script_dir, '*.jpeg'))

if not images:
    print("No images found for calibration in", script_dir)
    exit(1)

for filename in images:
    image = cv2.imread(filename)
    grayColor = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Find the chess board corners
    # If desired number of corners are
    # found in the image then ret = true
    ret, corners = cv2.findChessboardCorners(
        grayColor, CHECKERBOARD,
        cv2.CALIB_CB_ADAPTIVE_THRESH
        + cv2.CALIB_CB_FAST_CHECK +
        cv2.CALIB_CB_NORMALIZE_IMAGE)

    # If desired number of corners can be detected then,
    # refine the pixel coordinates and display
    # them on the images of checker board
    if ret == True:
        threedpoints.append(objectp3d)

        # Refining pixel coordinates
        # for given 2d points.
        corners2 = cv2.cornerSubPix(
            grayColor, corners, (11, 11), (-1, -1), criteria)

        twodpoints.append(corners2)

        # Draw and display the corners
        image = cv2.drawChessboardCorners(image,
                                          CHECKERBOARD,
                                          corners2, ret)

    cv2.imshow('img', image)
    cv2.waitKey(0)

cv2.destroyAllWindows()

h, w = image.shape[:2]

if not threedpoints:
    print("No chessboard corners found in any of the images!")
    exit(1)

# Perform camera calibration by
# passing the value of above found out 3D points (threedpoints)
# and its corresponding pixel coordinates of the
# detected corners (twodpoints)
ret, matrix, distortion, r_vecs, t_vecs = cv2.calibrateCamera(
    threedpoints, twodpoints, grayColor.shape[::-1], None, None)


# Displaying required output
print(" Camera matrix:")
print(matrix)

print("\n Distortion coefficient:")
print(distortion)

print("\n Rotation Vectors:")
print(r_vecs)

print("\n Translation Vectors:")
print(t_vecs)

# Camera matrix:
# [[3.02256614e+03 0.00000000e+00 1.57332550e+03]
#  [0.00000000e+00 2.99920319e+03 2.08568524e+03]
# [0.00000000e+00 0.00000000e+00 1.00000000e+00]]
#
# Distortion coefficient:
# [[ 0.11227764 -0.26654594  0.0102432   0.00393819 -0.01863315]]
#
# Rotation Vectors:
# (array([[0.6941849 ],
#         [0.60814025],
#         [1.52889918]]), array([[ 0.02662026],
#                                [-0.0991817 ],
#                                [ 1.55349761]]), array([[-0.53196288],
#                                                        [-0.9587161 ],
#                                                        [ 1.25968779]]))
#
# Translation Vectors:
# (array([[ 42.80590986],
#         [-49.42197548],
#         [185.88854762]]), array([[ 68.5421583 ],
#                                  [-65.64651562],
#                                  [231.28746167]]), array([[ 31.02571101],
#                                                           [-62.1650472 ],
#                                                           [318.09342831]]))
