import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

data_test_1 = {
    'Input': [1, 2, 4, 8, 16],
    'Time': [0.120, 0.939, 7.432, 59.59, 476.8]
}

df = pd.DataFrame(data_test_1)
ax = df.plot(kind='scatter', x='Input', y='Time', color='blue', label='Compute Time Trends')

slope, intercept = np.polyfit(df['Input'], df['Time'], 1)
ax.plot(df['Input'], slope * df['Input'] + intercept, color='red', label=f'Linear-fit ($y = {slope:.2f}x + {intercept:.2f}$)')

coefficients = np.polyfit(df['Input'], df['Time'], 2)
quadratic = np.poly1d(coefficients)
x_vals = np.linspace(df['Input'].min(), df['Input'].max(), 100)
y_vals = quadratic(x_vals)
plt.plot(x_vals, y_vals, color='green', label=f'Quadratic-fit ($y = {coefficients[0]:.2f}x^2 + {coefficients[1]:.2f}x + {coefficients[2]:.2f}$)')

coefficients_3 = np.polyfit(df['Input'], df['Time'], 3)
cubic = np.poly1d(coefficients_3)
x_vals_3 = np.linspace(df['Input'].min(), df['Input'].max(), 100)
y_vals_3 = cubic(x_vals_3)
plt.plot(x_vals_3, y_vals_3, color='orange', label=f'Cubic-fit ($y = {coefficients_3[0]:.2f}x^3 + {coefficients_3[1]:.2f}x^2 + {coefficients_3[2]:.2f}x + {coefficients_3[3]:.2f}$)')

plt.xlabel('Input Size')
plt.ylabel('Computation Time')
plt.title('Test 1 Algorithm Runtime Analysis')
plt.legend()
plt.show()
