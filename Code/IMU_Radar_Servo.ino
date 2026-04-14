function drawIMU(heading, roll, pitch) {
  const ctx = imuCtx;
  const w = imuCanvas.width;
  const h = imuCanvas.height;

  ctx.clearRect(0, 0, w, h);
  ctx.fillStyle = "#071018";
  ctx.fillRect(0, 0, w, h);

  const cx = w / 2;
  const cy = h / 2;
  const r = 150;

  // Compass outer ring
  ctx.strokeStyle = "#35566f";
  ctx.lineWidth = 4;
  ctx.beginPath();
  ctx.arc(cx, cy, r, 0, Math.PI * 2);
  ctx.stroke();

  // Compass ticks
  for (let a = 0; a < 360; a += 30) {
    const rad = (a - 90) * Math.PI / 180;
    const x1 = cx + Math.cos(rad) * (r - 10);
    const y1 = cy + Math.sin(rad) * (r - 10);
    const x2 = cx + Math.cos(rad) * r;
    const y2 = cy + Math.sin(rad) * r;

    ctx.strokeStyle = "#8fb7ff";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(x1, y1);
    ctx.lineTo(x2, y2);
    ctx.stroke();
  }

  // Compass labels
  ctx.fillStyle = "#e8f0ff";
  ctx.font = "22px Arial";
  ctx.fillText("N", cx - 8, cy - r - 15);
  ctx.fillText("S", cx - 7, cy + r + 28);
  ctx.fillText("W", cx - r - 28, cy + 8);
  ctx.fillText("E", cx + r + 12, cy + 8);

  // Heading needle
  const hRad = (heading - 90) * Math.PI / 180;
  const hx = cx + Math.cos(hRad) * (r - 25);
  const hy = cy + Math.sin(hRad) * (r - 25);

  ctx.strokeStyle = "#ff3b3b";
  ctx.lineWidth = 5;
  ctx.beginPath();
  ctx.moveTo(cx, cy);
  ctx.lineTo(hx, hy);
  ctx.stroke();

  ctx.fillStyle = "#ff3b3b";
  ctx.beginPath();
  ctx.arc(cx, cy, 6, 0, Math.PI * 2);
  ctx.fill();

  // Tilt indicator box
  const boxX = 80;
  const boxY = 360;
  const boxW = 340;
  const boxH = 90;

  ctx.strokeStyle = "#35566f";
  ctx.lineWidth = 2;
  ctx.strokeRect(boxX, boxY, boxW, boxH);

  ctx.fillStyle = "#8fb7ff";
  ctx.font = "18px Arial";
  ctx.fillText("Roll / Pitch Indicator", boxX + 80, boxY - 10);

  // Map roll/pitch into box
  let dotX = boxX + boxW / 2 + (roll * 2.5);
  let dotY = boxY + boxH / 2 - (pitch * 2.0);

  dotX = Math.max(boxX + 10, Math.min(boxX + boxW - 10, dotX));
  dotY = Math.max(boxY + 10, Math.min(boxY + boxH - 10, dotY));

  // Center crosshair
  ctx.strokeStyle = "#45627c";
  ctx.beginPath();
  ctx.moveTo(boxX + boxW / 2, boxY);
  ctx.lineTo(boxX + boxW / 2, boxY + boxH);
  ctx.stroke();

  ctx.beginPath();
  ctx.moveTo(boxX, boxY + boxH / 2);
  ctx.lineTo(boxX + boxW, boxY + boxH / 2);
  ctx.stroke();

  // Tilt dot
  ctx.fillStyle = "#00e0ff";
  ctx.beginPath();
  ctx.arc(dotX, dotY, 10, 0, Math.PI * 2);
  ctx.fill();

  // Readout
  ctx.fillStyle = "#e8f0ff";
  ctx.font = "20px Arial";
  ctx.fillText("Heading: " + heading.toFixed(1) + "°", 150, 40);
  ctx.fillText("Roll: " + roll.toFixed(1) + "°", 40, 485);
  ctx.fillText("Pitch: " + pitch.toFixed(1) + "