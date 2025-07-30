function performSearch() {
    const query = document.getElementById('searchInput').value;
    const resultsDiv = document.getElementById('results');
    resultsDiv.innerHTML = 'Loading...';

    if (!query) {
        resultsDiv.innerHTML = 'Please enter a search term.';
        return;
    }

    fetch(`https://api.duckduckgo.com/?q=${encodeURIComponent(query)}&format=json&no_redirect=1&no_html=1`)
    .then(response => response.json())
    .then(data => {
        resultsDiv.innerHTML = '';

        if (data.AbstractText) {
            resultsDiv.innerHTML = `<p><strong style="color:#00cc99">${data.heading}</strong>: ${data.AbstractText}</p>`;
        } else if (data.RelatedTopics.length > 0) {
            data.RelatedTopics.slice(0, 5).forEach(topic => {
                if (topic.Text && topic.FirstURL) {
                    resultsDiv.innerHTML += `<p><a href="${topic.FirstURL}" target="_blank">${topic.Text}</a></p>`;
                } else if (topic.Topics) {
                    topic.Topics.slice(0, 3).forEach(sub => {
                        if (sub.Text && sub.FirstURL) {
                            resultsDiv.innerHTML += `<p><a href="${sub.FirstURL}" target="_blank">${sub.Text}</a></p>`;
                        }
                    });
                }
            });
        } else {
            resultsDiv.innerHTML = 'No results found.';
        }
    })
    .catch(error => {
        console.error('Search error: ', error);
        resultsDiv.innerHTMl = 'Failed to fetch search results.';
    });
}