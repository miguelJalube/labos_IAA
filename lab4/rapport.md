# Lab4 - Miguel Jalube & Michael Gogniat - 13 mai 2024
## Questions
**Record the range of the weights, as well as their 3-sigma range (the difference between μ−3σ and μ+3σ)**
- Range of the weights : [-0.1924 ; 0.2263]
- sigma : 0.0429
- mean : -0.0024
- 3-sigma range : -0.0024 - 3 * 0.0429 = -0.1311 ; -0.0024 + 3 * 0.0429 = 0.1263
                    = [-0.1311 ; 0.1263]
**Explain which range you used for your quantization. Does range have an impact on model performance in this case ? Explain your answer.**
We used the 3-sigma range for our quantization. The range has an impact on the model performance because it determines the precision of the weights. If the range is too small, the weights will be quantized and will have less precision.

**Do you observe a drop in the general accuracy ? If you did everything right, it should be negligible. Explain your findings**
The accuracy didn't change from 54.61 to 54.95% when we quantized the weights.

**Compare the memory footprint of the original model and the quantized one. Did the memory footprint change ? Explain your findings. You can use torchinfo or torch-summary to get the memory footprint.**
The memory footprint didnt change when we quantized the weights.

## Stage 2
**Record the range of the activations, as well as their 3-sigma range.**

Input
Min weight:  -1.0
Max weight:  1.0
Mean:  -0.04456801918857207
Sigma:  0.5007391339605283
Mean - 3 sigma:  -1.5467854210701568
Mean + 3 sigma:  1.4576493826930128

Conv1
Min weight:  0.0
Max weight:  10.015969276428223
Mean:  0.5392608588071649
Sigma:  0.7935812573396501
Mean - 3 sigma:  -1.8414829132117854
Mean + 3 sigma:  2.9200046308261154

Conv2
Min weight:  0.0
Max weight:  15.086922645568848
Mean:  0.6547757158195966
Sigma:  1.0954099252933234
Mean - 3 sigma:  -2.6314540600603733
Mean + 3 sigma:  3.941005491699567

Fc1
Min weight:  0.0
Max weight:  10.567023277282715
Mean:  0.382443176894174
Sigma:  0.872996524804433
Mean - 3 sigma:  -2.2365463975191253
Mean + 3 sigma:  3.001432751307473

Fc2
Min weight:  0.0
Max weight:  6.216141223907471
Mean:  0.29708697306463616
Sigma:  0.6021290853042156
Mean - 3 sigma:  -1.5093002828480107
Mean + 3 sigma:  2.1034742289772828

Ffc3
Min weight:  -6.304019451141357
Max weight:  8.841646194458008
Mean:  0.023744911157715105
Sigma:  1.9384469447107846
Mean - 3 sigma:  -5.791595922974638
Mean + 3 sigma:  5.839085745290069


**Develop a formula for the quantized output of conv1 : O1q, as a function of sW 1, sO1, sI , Iq, W1q.**

(Not sure about the formula)

$$ O1q = \frac{SI(i - 1)\times SWi}{SOi} \times (I{(i-1)q \times Wiq}) $$

**Develop a general formula for the quantized output of convi : Oiq, as a function of sW i, sOi, sI , Iq, Wiq, and the scaling factors of the previous layers. Try to do it recursively by starting with conv2q to extract the formula**

$$ Stotal = SW1 \times SO1 \times SW2 \times SO2 \times ... \times SW(i-1) \times SO(i-1) \times SWi \times SOi $$

$$ Oiq = \frac{Stotal}{SOi} \times (I{(i-1)q \times Wiq}) $$

## Stage 3
**What’s the objective of the adjust command ?**

-

**With the help of the nnTool documentation 1, provide the right command for layer fusions.**

-

**Which layers have been fused ?**
-

**Based on the first parts of this lab, explain why we need a set of images for our quantization**

-

**What should be the properties of this set of images ? Think in terms of diversity of images**

-

**What should be the properties of this set of images ? Think in terms of diversity of images**

-