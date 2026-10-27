// ข้อมูลจำลองสำหรับ #explore — ไม่ใช่ค่าจากอุปกรณ์จริง
window.SPYNE_DEMO_STATES = [
  {
    "label": "นั่งตรง",
    "code": "UPRIGHT",
    "head": 55,
    "chest": 60,
    "description": "ระยะทั้งสองอยู่ในช่วงอ้างอิง ระบบใช้ไฟสีเขียว",
    "color": "#647d69"
  },
  {
    "label": "ใกล้เกินไป",
    "code": "TOO CLOSE",
    "head": 42,
    "chest": 47,
    "description": "ศีรษะและหน้าอกอยู่ใกล้กว่าเกณฑ์ที่ตั้งไว้",
    "color": "#a28b62"
  },
  {
    "label": "ไกลเกินไป",
    "code": "TOO FAR",
    "head": 70,
    "chest": 75,
    "description": "ศีรษะและหน้าอกอยู่ไกลกว่าเกณฑ์ที่ตั้งไว้",
    "color": "#a28b62"
  },
  {
    "label": "คอยื่น",
    "code": "HEAD FORWARD",
    "head": 43,
    "chest": 60,
    "description": "ศีรษะใกล้กว่าเกณฑ์ แต่หน้าอกยังอยู่ในช่วงอ้างอิง",
    "color": "#a28b62"
  },
  {
    "label": "ตัวไถล / หลังยุบ",
    "code": "SLIDE / SLUMP",
    "head": 55,
    "chest": 75,
    "description": "ตามโค้ดปัจจุบัน: หน้าอกไกลกว่าเกณฑ์ ขณะที่ศีรษะปกติหรือใกล้",
    "color": "#a28b62"
  },
  {
    "label": "ไม่ได้นั่ง",
    "code": "NOT SITTING",
    "head": 120,
    "chest": 125,
    "description": "ระยะทั้งสองเกินค่าอ้างอิงมากกว่า 50 cm ระบบใช้ไฟสีน้ำเงิน",
    "color": "#73899b"
  },
  {
    "label": "ไม่สมดุล",
    "code": "UNBALANCED",
    "head": 70,
    "chest": 48,
    "description": "รูปแบบที่ไม่เข้าเงื่อนไขหลัก โค้ดจัดเป็น UNBALANCED",
    "color": "#a28b62"
  }
];
