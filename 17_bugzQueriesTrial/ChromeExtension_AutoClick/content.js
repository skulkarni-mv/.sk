// Function to click the link
function clickLink() {
  // Query for the link by its text content
  const links = Array.from(document.querySelectorAll('a'));
  const targetLink = links.find(link => link.textContent.trim() === 'CSV');

  if (targetLink) {
    targetLink.click();
    console.log('Link clicked!');
  } else {
    console.log('Link not found.');
  }
}

// Wait for the page to fully load
window.addEventListener('load', clickLink);

