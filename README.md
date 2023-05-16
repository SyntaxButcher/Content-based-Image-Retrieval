# Content-based-Image-Retrieval

This project demonstrates an image similarity matching technique using OpenCV.

# Overview
The **main.cpp** file includes the main logic of the program. It uses OpenCV to read images, perform certain feature extraction techniques, and rank images based on similarity to a target image.

The **util.cpp** file includes functions for feature extraction, loading data, and calculating distances. It includes three types of features: 9x9 pixels from the middle of the image, a 3D histogram of the whole image, and multi histogram where the image is divided into two halves and histograms are calculated for both.

# Functionality

**Feature Extraction**: The program extracts features from the images for comparison. This can be done using three different techniques: using the 9x9 pixel values from the middle of the image, creating a 3D histogram of the image, or creating two histograms of the top and bottom half of the image.

**Image Comparison**: It compares the target image with other images in the dataset. The comparison is made by calculating the Sum of Squared Differences (SSD) or the histogram intersection, depending on the feature extraction method used.

**Ranking**: The program ranks all images in the dataset based on their similarity to the target image.

**Display**: Finally, it displays the top 'n' most similar images to the target image in a collage.

# How to Use
You can select the feature extraction method you want to use by changing the featureNum variable in the main function. You can also change the target image and the number of matches you want to display.

The code is designed to work with the Olympus dataset. To use the code, please download the Olympus dataset and store it in a folder named olympus in the same directory as your code.

# Results
The results are displayed in a collage of images, starting with the target image followed by the most similar images. The similarity is based on the selected feature extraction method and the comparison metric (SSD or histogram intersection).

Please note that this code requires OpenCV to be installed. Make sure to adjust the directory variable in the code to match your specific setup.

For instance: 

These are the top **4** matches for **pic.00274** in the olympus dataset using **multi-histogram with top half and bottom half** as features.

**Pic 00274**

<img src="https://github.com/SyntaxButcher/Content-based-Image-Retrieval/blob/main/Results/pic274.png" width="50%" height="50%">

**Top 4 matches**

<img src="https://github.com/SyntaxButcher/Content-based-Image-Retrieval/blob/main/Results/topNmatches.png" width="100%" height="100%">


