let primary = `var("--primary");
let secondary = `var("--secondary");
let accent = `var("--accent");

let foreground9 = `var("--foreground-9");
let foreground10 = `var("--foreground-10");
let foreground11 = `var("--foreground-11");

let background9 = `var("--background-9");
let background10 = `var("--background-10");
let background11 = `var("--background-11");

let success = `var("--success");
let failure = `var("--failure");
let warning = `var("--warning");
let info = `var("--info");

let afterSmall = {j|@media screen and (min-width: 640px)|j};
let afterMedium = {|@media screen and (min-width: 768px)|};
let afterLarge = {j|@media screen and (min-width: 1024px)|j};

let ghostButton = [%cx
  {|
display: flex;
align-items: center;
justify-content: center;

cursor: pointer;
background-color: $(background10);
border: 2px solid $(background9);
border-radius: 8px;
padding: 1rem;

transition:
  color 150ms ease-out,
  border-color 150ms ease-out;

&:hover {
  color: $(secondary);
  border-color: $(secondary);
}
|}
];

let primaryButton = [%cx
  {|
display: flex;
align-items: center;
justify-content: center;

cursor: pointer;
color: $(background10);
background-color: $(primary);
border: 2px solid $(primary);
border-radius: 8px;
padding: 1rem;

transition:
  color 150ms ease-out,
  background-color 150ms ease-out;

&:hover {
  color: $(primary);
  background-color: $(background10);
}
|}
];
