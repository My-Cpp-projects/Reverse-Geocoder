# Reverse-Geocoder
This is a program that accepts a file with coordinates and finds street name based on the coordinates

It uses http://www.mapquestapi.com/ to fetch the data of streets. One has to create an account (there are free options) in order to get a key.
The obtained key is required as an input to the program so it can fetch street addresses.

The program accepts files where the coordinate data is written in format: latitude longitude (without any extra characters)
The program outputs data (appending) into a file that user specifies in format: latitude longitude street_name
