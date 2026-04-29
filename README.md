🚀 CampusBot – Autonomous Smart Delivery Robot

CampusBot is a line-following autonomous delivery robot designed for smart campuses.
It uses vision-based decision making + mechanical delivery system to transport and deliver parcels without human intervention.

⸻

📌 Problem

In campuses and institutions, small deliveries (documents, items, lab materials):

* Waste time
* Require manual effort
* Are repetitive and inefficient

⸻

💡 Solution

CampusBot automates this using:

* 🤖 Line Following Navigation
* 👁️ Vision-Based Decision System
* 📦 Automated Delivery Mechanism
* 🔁 Return-to-Path Intelligence

⸻

⚙️ How It Works

🔄 Full Workflow

1. Robot follows black line
2. When all IR sensors detect black → STOP
3. MU Vision detects card:
    * ➡️ Right Card → Delivery
    * ⬅️ / ⬆️ → Continue path
4. Robot moves right to delivery zone
5. When all sensors detect white → STOP
6. Delivery starts:
    * 🔓 Servo opens gate
    * ⬆️ Lift motor raises tray
    * ⏳ Wait 3 seconds
    * ⬇️ Tray lowers
    * 🔒 Gate closes
7. Robot returns to line
8. Resumes normal line following

⸻

🧠 System Architecture

🔹 Hardware

* Arduino Uno
* L293D Motor Shield (AFMotor)
* 4x DC Motors (Omni/Mecanum Wheels)
* 5-Channel Analog IR Sensors
* MU Vision Sensor (Traffic Card Detection)
* Servo Motor (Gate Control)
* L298N Motor Driver (Lift System)
* N20 Gear Motor (Tray Lift)
* Buzzer
* Battery Pack

⸻

🔹 Software Logic

State-based control system:

Mode	Description
0	Line Following
1	Stop & Scan Card
2	Move to Delivery
3	Delivery Execution
4	Return to Line

⸻

🔧 Key Features

* ✔ Smooth and stable line following
* ✔ Vision-based routing (no GPS needed)
* ✔ Automated parcel delivery system
* ✔ Return-to-track mechanism
* ✔ Low-cost and scalable design

⸻

⚠️ Challenges & Solutions

Challenge	Solution
Sensor noise	Threshold tuning (≈300)
Unstable movement	Rule-based logic (no PID)
Servo jitter	Attach/Detach control
MU Vision interference	Trigger-based usage
Motor inconsistency	Speed calibration (~110)

⸻

📊 Technical Highlights

* Analog IR-based detection system
* Real-time decision making using vision
* Hybrid motor control (Shield + L298N)
* Sequential state machine logic

⸻

🔊 Feedback System

* 🔔 Beep on stop detection
* 🔔 Continuous beep during lift
* 🔔 Delivery confirmation sound

⸻

🚀 Future Improvements

* Multi-destination delivery
* Mobile app integration
* AI-based object detection
* Cloud monitoring system
* Battery optimization

⸻

🎯 Applications

* College campuses
* Office automation
* Hospitals (sample delivery)
* Warehouses

⸻

🏁 Conclusion

CampusBot proves that simple hardware + smart logic = powerful automation system.

It is:

* Practical
* Competition-ready
* Scalable for real-world use

⸻

👨‍💻 Author
Team - RoboxCraft
Team Lead - Sugam Saud
Robotics & AI Developer
NovaMate Project Builder

⸻

⭐ If you like this project

Give it a ⭐ on GitHub!
