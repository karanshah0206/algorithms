import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

data_test_1 = {
    'Input': [1, 2, 4, 8, 16],
    'Time': [0.120, 0.939, 7.432, 59.59, 476.8]
}

data_test_2 = {
    'Input': [1, 2, 4, 8, 16],
    'Time': [0.004, 0.007, 0.021, 0.07, 0.170]
}

df_test_1 = pd.DataFrame(data_test_1)
slope1, _ = np.polyfit(np.log2(df_test_1['Input']), np.log2(df_test_1['Time']), 1)

df_test_2 = pd.DataFrame(data_test_2)
slope2, _ = np.polyfit(np.log2(df_test_2['Input']), np.log2(df_test_2['Time']), 1)

plt.plot(df_test_1['Input'], df_test_1['Time'], marker='o', label=f'Test 1 (slope ≈ {slope1:.2f})')
plt.plot(df_test_2['Input'], df_test_2['Time'], marker='o', label=f'Test 2 (slope ≈ {slope2:.2f})')

plt.title('Runtime comparison of Test 1 and Test 2 Algorithms')
plt.xlabel('Log_2 Scaled Input Size')
plt.ylabel('Log_2 Scaled Computation Time (seconds)')
plt.yscale('log', base=2)
plt.xscale('log', base=2)
plt.legend()
plt.tight_layout()
plt.show()
