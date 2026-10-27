/* ======================================================================
   SPYNE — Source Code Viewer & Syntax Highlighter
   - ไฮไลต์ไวยากรณ์ Arduino / C++ แบบเนทีฟ (0 dependencies)
   - ดึงโค้ดล่าสุดจาก SPYNE/SPYNE.ino อัตโนมัติ
   - คัดลอกโค้ดต้นฉบับได้โดยไม่ติดแท็ก HTML
   ====================================================================== */

function highlightCpp(source) {
  const tokenRegex = /(\/\*[\s\S]*?\*\/|\/\/[^\r\n]*)|(#\s*include\s*(?:<[^>]+>|"[^"]+")|#\s*\w+)|("(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')|(\b0x[0-9a-fA-F]+\b|\b\d+(?:\.\d+)?\b)|(\b[a-zA-Z_]\w*\b)/g;

  const types = new Set([
    'void', 'int', 'long', 'unsigned', 'bool', 'boolean', 'char', 'float', 'double',
    'const', 'String', 'static', 'short', 'byte', 'word', 'KVStore',
    'BLEAgentClass', 'SerialAgentClass', 'WiFiConnectionHandler', 'NetworkConfiguratorClass',
    'LiquidCrystal_PCF8574', 'LiquidCrystal_I2C', 'size_t'
  ]);

  const keywords = new Set([
    'if', 'else', 'for', 'while', 'do', 'return', 'switch', 'case', 'break', 'continue',
    'default', 'new', 'delete', 'class', 'struct', 'enum', 'public', 'private', 'protected'
  ]);

  const constants = new Set([
    'true', 'false', 'HIGH', 'LOW', 'INPUT', 'OUTPUT', 'NULL', 'SECONDS',
    'READ', 'WRITE', 'READWRITE', 'ON_CHANGE'
  ]);

  const builtins = new Set([
    'Serial', 'ArduinoCloud', 'lcd', 'pinMode', 'digitalWrite', 'digitalRead',
    'analogWrite', 'analogRead', 'delay', 'delayMicroseconds', 'millis', 'tone',
    'pulseIn', 'abs', 'setBacklight', 'print', 'println', 'begin', 'clear',
    'setCursor', 'update', 'connected', 'setDebugMessageLevel', 'printDebugInfo',
    'addProperty', 'setConfigurator', 'addAgent', 'setStorage', 'readUltrasonicSensor',
    'readFiltered', 'runCalibration', 'computeState', 'classifyPosture',
    'displayLocalFeedback', 'setColor', 'initProperties', 'setup', 'loop'
  ]);

  function escapeHtml(str) {
    return str
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;');
  }

  let lastIndex = 0;
  let html = '';
  let match;

  while ((match = tokenRegex.exec(source)) !== null) {
    if (match.index > lastIndex) {
      html += escapeHtml(source.slice(lastIndex, match.index));
    }
    lastIndex = tokenRegex.lastIndex;

    const [raw, comment, preproc, str, num, word] = match;

    if (comment) {
      html += `<span class="token-comment">${escapeHtml(comment)}</span>`;
    } else if (preproc) {
      html += `<span class="token-preproc">${escapeHtml(preproc)}</span>`;
    } else if (str) {
      html += `<span class="token-string">${escapeHtml(str)}</span>`;
    } else if (num) {
      html += `<span class="token-number">${escapeHtml(num)}</span>`;
    } else if (word) {
      if (types.has(word)) {
        html += `<span class="token-type">${escapeHtml(word)}</span>`;
      } else if (keywords.has(word)) {
        html += `<span class="token-keyword">${escapeHtml(word)}</span>`;
      } else if (constants.has(word)) {
        html += `<span class="token-constant">${escapeHtml(word)}</span>`;
      } else if (builtins.has(word)) {
        html += `<span class="token-builtin">${escapeHtml(word)}</span>`;
      } else {
        html += escapeHtml(word);
      }
    }
  }

  if (lastIndex < source.length) {
    html += escapeHtml(source.slice(lastIndex));
  }

  return html;
}

// 01. คัดลอกโค้ดต้นฉบับ
const copyButton = document.querySelector('#copy-source');
const copyStatus = document.querySelector('#copy-status');

if (copyButton) {
  copyButton.addEventListener('click', async () => {
    const code = document.querySelector('#firmware-code');
    if (!code) return;

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

    setTimeout(() => {
      if (copyStatus) copyStatus.textContent = '';
    }, 3000);
  });
}

// 02. ไฮไลต์โค้ดเริ่มต้นที่ฝังอยู่ใน HTML
const initialCode = document.querySelector('#firmware-code');
if (initialCode && !initialCode.querySelector('.token-keyword, .token-comment, .token-preproc')) {
  initialCode.innerHTML = highlightCpp(initialCode.textContent);
}

// 03. ดึงไฟล์ SPYNE/SPYNE.ino ตัวจริงมาอัปเดตแบบเรียลไทม์
async function loadLiveSource() {
  try {
    const response = await fetch('SPYNE/SPYNE.ino');
    if (!response.ok) return;
    const text = await response.text();
    const codeElement = document.querySelector('#firmware-code');
    const lineCountElement = document.querySelector('#source-line-count');
    if (codeElement && text) {
      codeElement.innerHTML = highlightCpp(text);
    }
    if (lineCountElement && text) {
      const lineCount = text.trim().split('\n').length;
      lineCountElement.textContent = lineCount;
    }
  } catch {
    // fallback โค้ดที่เรนเดอร์ไว้ใน HTML หากเปิดผ่าน file://
  }
}

loadLiveSource();
