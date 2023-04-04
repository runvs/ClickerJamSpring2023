function save(data) {
    console.log('Saving game data...');
    localStorage.setItem('SaveGame', JSON.stringify(data));
}

function load() {
    console.log('Loading game data...');
    return localStorage.getItem('SaveGame') || '';
}
