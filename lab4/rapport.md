# Lab4 - Miguel Jalube & Michael Gogniat - 13 mai 2024
## Questions
**Record the range of the weights, as well as their 3-sigma range (the difference between μ−3σ and μ+3σ)**
- σ : 0.04
- μ : 0.5
- 3-sigma range : 0.38 - 0.62
**Explain which range you used for your quantization. Does range have an impact on model performance in this case ? Explain your answer.**
We used the 3-sigma range for our quantization. The range has an impact on the model performance because it determines the precision of the weights. If the range is too small, the weights will be quantized with a high precision, but the model will be more sensitive to noise. If the range is too large, the weights will be quantized with a low precision, but the model will be less sensitive to noise. In this case, the 3-sigma range is a good compromise between precision and noise sensitivity.

**Do you observe a drop in the general accuracy ? If you did everything right, it should be negligible. Explain your findings**
The accuracy droped from 52.6% to 10% when we quantized the weights. The drop in accuracy is not negligible.

**Compare the memory footprint of the original model and the quantized one. Did the memory footprint change ? Explain your findings. You can use torchinfo or torch-summary to get the memory footprint.**
The memory footprint didnt change when we quantized the weights.

## Stage 2
**Record the range of the activations, as well as their 3-sigma range.**

**Develop a formula for the quantized output of conv1 : O1q, as a function of sW 1, sO1, sI , Iq, W1q.**

**Develop a general formula for the quantized output of convi : Oiq, as a function of sW i, sOi, sI , Iq, Wiq, and the scaling factors of the previous layers. Try to do it recursively by starting with conv2q to extract the formula**