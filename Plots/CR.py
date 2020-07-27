import matplotlib.pyplot as plt 

#caida
x1 = [3, 4, 5, 6, 7, 8, 9, 10]
y1 = [0.987594, 0.975435, 0.960744, 0.941835, 0.921353, 0.898014, 0.875723, 0.860068]
plt.plot(x1, y1, label = "caida")

#livemocha
x2 = [3, 4, 5, 6, 7, 8, 9, 10]
y2 = [0.99243, 0.992681, 0.972357, 0.926914, 0.8612, 0.773969, 0.653299, 0.548439]
plt.plot(x2, y2, label = "livemocha")

#amazon
x3 = [3, 4, 5, 6, 7, 8, 9, 10]
y3 = [0.952122, 0.958814, 0.918115, 0.824104, 0.693343, 0.594514, 0.527526, 0.478059]
plt.plot(x3, y3, label = "amazon")

#hyves
x4 = [3, 4, 5, 6, 7, 8, 9, 10]
y4 = [0.426243, 0.820831, 0.966926, 0.886641, 0.850357, 0.916139, 0.932233, 0.912209]
plt.plot(x4, y4, label = "hyves")

#com-dblp
x5 = [3, 4, 5, 6, 7, 8, 9, 10]
y5 = [0.959622, 0.975492, 0.95414, 0.902172, 0.809323, 0.70428, 0.606274, 0.519161]
plt.plot(x5, y5, label = "com-dblp")

#wordnet
x6 = [3, 4, 5, 6, 7, 8, 9, 10]
y6 = [0.98494, 0.977745, 0.937768, 0.87035, 0.780051, 0.689252, 0.606935, 0.543652]
plt.plot(x6, y6, label = "wordnet")

#protein
x7 = [3, 4, 5, 6, 7, 8, 9, 10]
y7 = [0.868985, 0.838235, 0.763007, 0.716179, 0.680496, 0.599788, 0.509742, 0.511892]
plt.plot(x7, y7, label = "protein")

#dolphins
x8 = [3, 4, 5]
y8 = [0.392405, 0.215686, 0.148148]
plt.plot(x8, y8, label = "dolphins")

# naming the x axis 
plt.xlabel('Community size') 
# naming the y axis 
plt.ylabel('Compression rate') 
# giving a title to my graph 
plt.title('Community size  x Compression rate') 
  
# show a legend on the plot 
plt.legend() 
  
# function to show the plot 
plt.show() 