var socketUrl = "ws://localhost:8080/_livereload";
var webSocket = new WebSocket(socketUrl);

webSocket.onopen = () => {
    console.debug("Live Reload: Connected");
};

webSocket.onclose = () => {
    console.log("Live Reload: Closed");

    const reload = () => {
        reloadWebSocket = new WebSocket(socketUrl);

        reloadWebSocket.onerror = (_event) => {
            setTimeout(reload, 500);
        };

        reloadWebSocket.onopen = (_event) => {
            location.reload();
        };
    }

    reload();
};

webSocket.onerror = (event) => {
    console.debug("Live Reload: Websocket Error", event);
}
