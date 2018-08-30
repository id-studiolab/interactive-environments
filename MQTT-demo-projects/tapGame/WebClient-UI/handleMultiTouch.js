(function() {
    var lastTouch = 0;
    function preventZoom(e) {
        var t2 = e.timeStamp;
        var t1 = lastTouch || t2;
        var dt = t2 - t1;
        var fingers = e.touches.length;
        lastTouch = t2;

        if (!dt || dt >= 300 || fingers > 1) {
            return;
        }
        resetPreventZoom();
        e.preventDefault();
        e.target.click();
    }
    function resetPreventZoom() {
        lastTouch = 0;
    }

    document.addEventListener('touchstart', preventZoom, false);
    document.addEventListener('touchmove', resetPreventZoom, false);
})();
