const fs = require('fs');

const files = [
  {filename: 'log_summary_dev_a.txt', name: 'p=0.00'},
  {filename: 'log_summary_dev_b.txt', name: 'p=0.01'}
];

const statsHeader = ['avg', 'sd', 'min', 'max'];

const header1 = ['', '', 
      'score', ...statsHeader.map(x => '').slice(1),
      'time', ...statsHeader.map(x => '').slice(1)
];
const header2 = ['filename', 'n', 
      ...statsHeader, 
      ...statsHeader
];
let table = [header1, header2];

files.forEach(({filename, name}) => {
  const rows = fs.readFileSync(filename, 'utf-8').split('\n');
  const ts = rows
    .map(row => row
      .split(',')
      .map(row => row.trim())
      .map(row => row.split(' '))
    )
    .filter(x => x);
  const scores = ts
    .map(t => t.find(row => row[0] === 'Score:'))
    .filter(x => x)
    .map(x => Number(x[1]));
  const times = ts
    .map(t => t.find(row => row[0] === 'Time:'))
    .filter(x => x)
    .map(x => Number(x[1]));

  const scoreStats = stats(scores);
  const timeStats = stats(times);

  console.assert(scoreStats.length === timeStats.length);

  const n = scores.length;

  table.push([name, n, ...scoreStats, ...timeStats]);
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

const padN = Object.keys(header1).map(i => Math.max(...table.map(x => String(x[i]).length)));
table.forEach(row => {
  const text = row.map((x, i) => x.padStart(padN[i])).join('  ');
  console.log(text);
});

function floorText(x) {
  return Math.floor(x * 100) / 100;
}

function stats(xs) {
  const n = xs.length;
  const sum = xs.reduce((y, x) => y + x, 0);
  const avg = sum / n;
  const vari = xs.reduce((y, x) => y + Math.pow(x - avg, 2), 0) / n;
  const sd = Math.sqrt(vari);
  const min = Math.min(...xs);
  const max = Math.max(...xs);

  return [avg, sd, min, max];
}
