export const quotes = [
    "ES module is cool",
    "Sorry Python",
    "C got outsmarted",
    "JS: I got import/export",
    "Python: Why you like this..."
];

export function getRandomQuote() {
    const i = Math.floor(Math.random() * quotes.length);
    return quotes[i];
}