document.getElementById('searchBtn').addEventListener('click', searchWiki);

function searchWiki() {
    const query = document.getElementById('searchInput').value.trim();
    if (!query) {
        alert("Please enter a search term.");
        return;
    }

    const url = `https://en.wikipedia.org/w/api.php?origin=*&action=query&list=search&srsearch=${encodeURIComponent(query)}&format=json`;

    fetch(url)
      .then(response => response.json())
      .then(data => {
        const resultsDiv = document.getElementById('results');
        resultsDiv.innerHTML = '';

        const results = data.query.search;
        if (results.length === 0) {
            resultsDiv.innerHTML = '<p>No results found.</p>';
            return;
        }

        results.forEach(result => {
            const resultElement = document.createElement('div');
            resultElement.className = 'result';
            resultElement.innerHTML = `
              <h3><a href="https://en.wikipedia.org/?curid=${result.pageid}" target="_blank">${result.title}</a></h3>
              <p>${result.snippet}...</p>
            `;
            resultsDiv.appendChild(resultElement);
        });
      })
      .catch(error => {
        console.error("Error: ", error);
      });
}