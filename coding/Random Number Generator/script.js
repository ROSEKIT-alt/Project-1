function generateBtn() {
    const randomNumber = Math.floor(Math.random() * 100) + 1;
    document.getElementById("output").textContent = randomNumber;
}