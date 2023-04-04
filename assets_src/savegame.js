function save(data) {
    console.log('Saving game data...');
    localStorage.setItem('SaveGame', data.toString());
}

function load() {
    console.log('Loading game data...');
    return JSON.stringify(localStorage.getItem('SaveGame')) || '';
}
