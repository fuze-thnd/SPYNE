/* คัดลอก Source Code ที่แสดงอยู่ในหน้าเว็บ โดยไม่แก้ไฟล์ต้นฉบับ */
const copyButton = document.querySelector('#copy-source');
const copyStatus = document.querySelector('#copy-status');

copyButton.addEventListener('click', async () => {
  const code = document.querySelector('#firmware-code');

  try {
    await navigator.clipboard.writeText(code.textContent);
    copyStatus.textContent = 'คัดลอกโค้ดแล้ว';
  } catch {
    const selection = window.getSelection();
    const range = document.createRange();
    range.selectNodeContents(code);
    selection.removeAllRanges();
    selection.addRange(range);
    copyStatus.textContent = 'เลือกโค้ดแล้ว กด Ctrl+C หรือใช้เมนูคัดลอก';
  }
});
