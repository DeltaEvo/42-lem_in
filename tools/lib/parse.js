module.exports = function parseMap(content) {
    const lines = content.split("\n").filter(line => line.length);

    const rooms = {};
    const links = [];
    const turns = [];

    let start = false;
    let end = false;
    for (const line of lines.slice(1)) {
        const parts = line.split(" ");

        // Room: name x y
        if (line.startsWith("L")) {
            turns.push(parts
                .map(part => part.slice(1))
                .map(part => part.split("-", 2))
                .map(([ant, room]) => ({ ant: + ant, room })))
        }
        else if (parts.length == 3) {
            const [name, x, y] = parts;
            rooms[name] = {
                name,
                x: +x,
                y: +y,
                start,
                end
            };
            start = false;
            end = false;
        } else if (parts.length == 1) {
            if (line.startsWith("#")) {
                // Comment
                if (line === "##start") start = true;
                if (line === "##end") end = true;
            } else {
                // Link
                const names = line.split("-", 2);
                if (names.length != 2) throw new Error("Invalid link");
                const room1 = rooms[names[0]];
                const room2 = rooms[names[1]];
                if (!room1 || !room2) throw new Error("Unknown room in link");
                links.push([room1, room2]);
            }
        }
    }

    return {
        ants: +lines[0],
        rooms,
        links,
        turns
    };
}