const fs = require('fs');

const files = [
  {filename: 'log_summary_ox_init1_pm10.txt', name: 'ox_pm0.10'},
  {filename: 'log_summary_ox_init1_pm20.txt', name: 'ox_pm0.20'},
  {filename: 'log_summary_ox_init1_pm30.txt', name: 'ox_pm0.30'},
  {filename: 'log_summary_ox_init1_pm50.txt', name: 'ox_pm0.50'},
  {filename: 'log_summary_ox_init1_pm60.txt', name: 'ox_pm0.60'},
  {filename: 'log_summary_ox_init1_pm80.txt', name: 'ox_pm0.80'},
  {filename: 'log_summary_ox_init1.txt', name: 'ox_pm1.00'},

  {filename: 'log_summary_er_init1_pm10.txt', name: 'er_pm0.10'},
  {filename: 'log_summary_er_init1_pm20.txt', name: 'er_pm0.20'},
  {filename: 'log_summary_er_init1_pm30.txt', name: 'er_pm0.30'},
  {filename: 'log_summary_er_init1_pm50.txt', name: 'er_pm0.50'},
  {filename: 'log_summary_er_init1_pm60.txt', name: 'er_pm0.60'},
  {filename: 'log_summary_er_init1_pm80.txt', name: 'er_pm0.80'},
  {filename: 'log_summary_er_init1.txt', name: 'er_pm1.00'},

  {filename: 'log_summary_er_init1_pm10.txt_2', name: 'er_pm0.10_bef'},
  {filename: 'log_summary_er_init1_pm20.txt_2', name: 'er_pm0.20_bef'},
  {filename: 'log_summary_er_init1_pm30.txt_2', name: 'er_pm0.30_bef'},
  {filename: 'log_summary_er_init1_pm50.txt_2', name: 'er_pm0.50_bef'},
  {filename: 'log_summary_er_init1_pm60.txt_2', name: 'er_pm0.60_bef'},
  {filename: 'log_summary_er_init1_pm80.txt_2', name: 'er_pm0.80_bef'},
  {filename: 'log_summary_er_init1.txt_2', name: 'er_pm1.00_bef'},
];

const header = ['filename', 'n', 'avg', 'sd', 'min', 'max'];
let table = [header];

files.forEach(({filename, name}) => {
  const rows = fs.readFileSync(filename, 'utf-8').split('\n');
  const scores = rows
    .map(row => row
      .split(',')
      .map(row => row.trim())
      .map(row => row.split(': '))
      .filter(row => row[0] === 'Score')
      [0]
    )
    .filter(x => x)
    .map(x => Number(x[1]));

  const n = scores.length;
  const sum = scores.reduce((y, x) => y + x, 0);
  const avg = sum / n;
  const vari = scores.reduce((y, x) => y + Math.pow(x - avg, 2), 0) / n;
  const sd = Math.sqrt(vari);
  const min = Math.min(...scores);
  const max = Math.max(...scores);

  table.push([name, n, avg, sd, min, max]);
});

table = table.map(row => {
  return row.map((x, i) => {
    if (i !== 1 && typeof x === 'number') {
      x = String(Math.floor(x * 100) / 100);
      const len = x.length;
      const pos = x.indexOf('.');
      if (pos === -1) {
        return x = x + '.00';
      } else if (pos + 2 === len) {
        return x = x + '0';
      }
    }
    return String(x);
  });
});

const padN = Object.keys(header).map(i => Math.max(...table.map(x => String(x[i]).length)));
table.forEach(row => {
  const text = row.map((x, i) => x.padStart(padN[i])).join('  ');
  console.log(text);
});

function floorText(x) {
  return Math.floor(x * 100) / 100;
}

