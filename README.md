# Physical Computing Project 2026 - IT KMITL | SPYNE

> **Disclaimer:** This project was developed as an educational project for the Physical Computing course at the School of Information Technology, KMITL. It is not for commercial use, production deployment, or sale.

## Objectives

เพื่อตรวจจับและจำแนกท่าทางการนั่งทำงาน โดยใช้ Ultrasonic Sensor 2 ระดับ (ระดับสายตาและระดับหน้าอก) ที่ติดตั้งบริเวณหน้าจอคอมพิวเตอร์ โดยออกแบบให้จำแนกท่าทางการนั่งได้ 6 รูปแบบ (นั่งตรง, ใกล้เกินไป, ไกลเกินไป, คอยื่น, ตัวไถล, และไม่ได้นั่ง) โดยไม่ต้องพึ่งพากล้อง

เพื่อแก้ไขท่านั่งของผู้ใช้ เมื่อตรวจพบพฤติกรรมการนั่งที่ผิดสรีระศาสตร์ติดต่อกันเกินเวลาที่กำหนด จะแจ้งเตือนผู้ใช้งาน ช่วยให้ผู้ใช้งานสามารถปรับเปลี่ยนท่าทางได้ทันที


## Overview

Project ต้องการแก้ปัญหานั่งทำงานไม่ถูกท่า ซึ่งส่งผลต่อกระดูกสันหลังและสุขภาพในระยะยาว จากผลสำรวจในปี 1990 ถึง 2015 พบอัตราการเพิ่มขึ้นของผู้ที่มีอาการปวดหลังสูงขึ้นถึง 54% Project นี้จึงมีประโยชน์ต่อนักเรียน, นักศึกษา, และวัยทำงานที่ใช้งานคอมพิวเตอร์เป็นเวลานาน โดยใช้ Ultrasonic Sensor ตัวแรกระดับสายตาบริเวณเหนือหน้าจอในการวัดระยะห่างจากตาถึงหน้าจอ และตัวที่สองอยู่บริเวณใต้หน้าจอในระดับหน้าอก เพื่อการจำแนกท่านั่ง (นั่งตรง/ใกล้เกิน/ไกลเกิน/คอยื่นไปข้างหน้า/ตัวไถล/ไม่ได้นั่ง) โดยมี

<code>Input: ข้อมูลจาก Ultrasonic Sensor และผู้ใช้งานกำหนดค่า</code>  
<code>Output: สถานะท่านั่ง, การแจ้งเตือนผ่าน RGB LED, LCD, และ Buzzer, และข้อมูลย้อนหลังผ่าน Arduino Cloud</code>  

#### Hardware

- Arduino UNO R4 WiFi 
- RGB LED (Tri-color LED) 
- HC-SR04 Ultrasonic Sensor ×2 
- Passive Buzzer
- LCD 16×2 with I2C Module
- Breadboard, Jumper Wires, และ Resistors


#### ตำแหน่งของ Sensor

- Eye-level Sensor: ติดตั้งบริเวณขอบบนของหน้าจอคอมพิวเตอร์ ทำมุมเล็งมาที่ใบหน้าหรือคาง เพื่อตรวจจับระยะห่างสายตาและหน้าจอ
- Chest-level Sensor: ติดตั้งบริเวณฐานหน้าจอหรือระดับโต๊ะ ทำมุมเล็งมาที่หน้าอกหรือกระดูกสันหลังส่วนบน เพื่อตรวจจับตำแหน่งลำตัวหลักและการไถลตัว


#### การจำแนกท่าทาง

| State | Eye-level Sensor	| Chest-level Sensor	|
|-------|------------------|--------------------|
| UPRIGHT | normal | normal |
| TOO CLOSE | closer | closer |
| TOO FAR | farther | farther |
| FORWARD HEAD | closer | normal |
| SLIDE/SLUMP | farther or not detected | farther |
| NOT SEATED | not detected | not detected |
| UNBALANCED | else | else |


### Source Code

- Arduino Sketch: [SPYNE/SPYNE.ino](SPYNE/SPYNE.ino)
- Arduino Cloud Properties: [SPYNE/thingProperties.h](SPYNE/thingProperties.h)

### Website

https://fuze-thnd.github.io/SPYNE/

### Demo Video

https://youtu.be/yrfo-nZxjXc

## Contributors

| ID | Name | Role & Responsibility | Img |
|---|---|---|---|
| 68070002 | นายกฤตชัย ถาวรพันธุ์ (เก้า) | **Hardware & Poster (33%)**<br>• เชื่อมต่ออุปกรณ์<br>• ทำอุปกรณ์จับยึดหน้าจอเข้ากับ Ultrasonic Sensor<br>• ออกแบบโปสเตอร์ A1 | <img height="150" src="assets/0002.jpg"> |
| 68070057 | นายธนดล กระจ่างโรจน์ (ฟิวส์) | **Software & Arduino Cloud (33%)**<br>• Algorithm จำแนกท่านั่ง<br>• แสดงผลผ่าน RGB LED, LCD, Buzzer<br>• เชื่อมข้อมูลจาก Arduino มายัง Arduino Cloud | <img height="150" src="assets/0057.png"> |
| 68070029 | นายชินดนัย เรืองสมบัติ (อู๋) | **Data Analyst & Web development (34%)**<br>• นำข้อมูลที่เก็บมาประมวลผล เพื่อสร้างกราฟสรุปพฤติกรรมในแต่ละวัน<br>• พัฒนาและ Deploy เว็บไซต์โปรเจกต์ผ่าน GitHub.io | <img height="150" src="assets/0029.jpg"> |
