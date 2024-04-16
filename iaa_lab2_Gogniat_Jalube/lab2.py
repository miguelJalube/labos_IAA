# lab2.py
import numpy as np
import torch
from PIL import Image
from torchvision import transforms
import segmentation_models_pytorch as smp

DEVICE = 'cpu'

class Pathfinder:
    def __init__(self):
        self.model = smp.Unet(encoder_name="resnet34", encoder_weights=None, classes=5, in_channels=1)
        checkpoint = torch.load("best_model.pth", map_location=DEVICE)
        self.model.load_state_dict(checkpoint)
        self.model.to(device=DEVICE)
        self.model.eval()

    def detect_line(self, image):
        image = Image.fromarray(np.uint8(image[0]*255))
        RESIZE = (128, 128)
        image_transforms = transforms.Compose([
            transforms.Grayscale(),
            transforms.Resize(RESIZE),
            transforms.ToTensor()
        ])
        image = image_transforms(image).unsqueeze(0)

        image.to(device=DEVICE)

        with torch.no_grad():
            logit_mask = self.model(image)
        prob_mask = torch.softmax(logit_mask, dim=1)
        lane_activation = prob_mask[:, 3, :, :].squeeze().cpu()

        portion_mask = lane_activation[124:127, 1:127]
        max_activation_portion_index = torch.argmax(portion_mask)
        max_activation_portion_coords = torch.unravel_index(max_activation_portion_index, portion_mask.shape)

        portion_mask2 = lane_activation[114:117, 1:127]
        max_activation_portion_index2 = torch.argmax(portion_mask2)
        max_activation_portion_coords2 = torch.unravel_index(max_activation_portion_index2, portion_mask.shape)

        points = [max_activation_portion_coords[1].item(),max_activation_portion_coords[0].item()+124,
                  max_activation_portion_coords2[1].item(),max_activation_portion_coords2[0].item()+114]
        return [int(coord * (200.0/128.0)) for coord in points]

def init():
    return Pathfinder()
