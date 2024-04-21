let primary = `var("--primary");
let secondary = `var("--secondary");
let accent = `var("--accent");
let foreground = `var("--foreground");
let foregroundSubtle = `var("--foreground-subtle");
let background = `var("--background");
let backgroundSubtle = `var("--background-subtle");

let afterSmall = {j|@media screen and (min-width: 640px)|j};
let afterMedium = {|@media screen and (min-width: 768px)|};
let afterLarge = {j|@media screen and (min-width: 1024px)|j};

let ghostButton = [%cx
  {|
display: flex;
align-items: center;
justify-content: center;

border: 1px solid $(backgroundSubtle);
border-radius: 8px;
padding: 1rem;
|}
];

let primaryButton = [%cx
  {|
display: flex;
align-items: center;
justify-content: center;

color: $(background);
background-color: $(primary);
border: 1px solid $(primary);
border-radius: 8px;
padding: 1rem;
|}
];
