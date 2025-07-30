import { getRandomQuote } from "./alt.js";

const quoteEl = document.getElementById('quote');
const newQuoteBtn = document.getElementById('new-quote');

function showQuote() {
    quoteEl.textContent = getRandomQuote();
}

newQuoteBtn.addEventListener('click', showQuote);

// Show first quote on load
showQuote();