
const LS_KEY = 'personal_expenses_v1';

function loadExpenses() {
  const raw = localStorage.getItem(LS_KEY);
  return raw ? JSON.parse(raw) : [];
}

function saveExpenses(arr) {
  localStorage.setItem(LS_KEY, JSON.stringify(arr));
}

function addExpenseObj(obj) {
  const arr = loadExpenses();
  arr.push(obj);
  saveExpenses(arr);
}


document.addEventListener('DOMContentLoaded', () => {
  const path = location.pathname.split('/').pop();
  if (path === '' || path === 'index.html') return;

  if (path === 'add.html') {
    const form = document.getElementById('expenseForm');
    const msg = document.getElementById('msg');
    const clearBtn = document.getElementById('clearBtn');

    form.addEventListener('submit', e => {
      e.preventDefault();
      const obj = {
        category: document.getElementById('category').value || 'Other',
        amount: parseFloat(document.getElementById('amount').value || 0),
        date: document.getElementById('date').value || new Date().toISOString().slice(0,10),
        note: document.getElementById('note').value || ''
      };
      if (!obj.amount || obj.amount <= 0) {
        msg.textContent = 'Please enter a valid amount.';
        return;
      }
      addExpenseObj(obj);
      msg.textContent = 'Expense added successfully!';
      form.reset();
      setTimeout(()=>msg.textContent='',2000);
    });

    clearBtn.addEventListener('click', () => form.reset());
  }

  if (path === 'view.html') {
    const tbody = document.querySelector('#expenseTable tbody');
    const totalEl = document.getElementById('totalAmt');
    const search = document.getElementById('search');
    const filterCategory = document.getElementById('filterCategory');
    const exportBtn = document.getElementById('exportBtn');
    const clearAll = document.getElementById('clearAll');

    function render() {
      const data = loadExpenses();
      const q = (search.value || '').toLowerCase();
      const cat = filterCategory.value || '';
      const filtered = data.filter((d) => {
        if (cat && d.category !== cat) return false;
        if (q && !(d.category.toLowerCase().includes(q) || d.note.toLowerCase().includes(q))) return false;
        return true;
      });
      tbody.innerHTML = '';
      let total = 0;
      filtered.forEach((d,i) => {
        total += Number(d.amount);
        const tr = document.createElement('tr');
        tr.innerHTML = `<td>${i+1}</td><td>${d.category}</td><td>${Number(d.amount).toFixed(2)}</td><td>${d.date}</td><td>${d.note}</td>`;
        tbody.appendChild(tr);
      });
      totalEl.textContent = Number(total).toFixed(2);
    }

    search.addEventListener('input', render);
    filterCategory.addEventListener('change', render);
    exportBtn.addEventListener('click', () => {
      const data = loadExpenses();
      const blob = new Blob([JSON.stringify(data,null,2)], {type:'application/json'});
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url; a.download = 'expenses_backup.json'; document.body.appendChild(a); a.click();
      a.remove(); URL.revokeObjectURL(url);
    });

    clearAll.addEventListener('click', () => {
      if (!confirm('Clear ALL expenses from this browser?')) return;
      localStorage.removeItem(LS_KEY);
      render();
    });

    render();
  }
});
