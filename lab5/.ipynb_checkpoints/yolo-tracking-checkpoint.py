from inference.models.yolo_world.yolo_world import YOLOWorld
import supervision as sv
import cv2

model = YOLOWorld( model_id="yolo_world/l")

classes = ["person"]

model.set_classes(classes)

def callback(frame: np.ndarray, index: int) -> np.ndarray:
    results = model.infer(frame, text=classes)
    detections = sv.Detections.from_inference(results)
    detections = byte_tracker.update_with_detections(detections)
    labels = [
        f"#{tracker_id} {model.model.names[class_id]} {confidence:0.2f}"
       for _, _, confidence, class_id, tracker_id
      in detections
    ]
    return annotator.annotate(scene=frame.copy(), detections=detections, labels=labels)