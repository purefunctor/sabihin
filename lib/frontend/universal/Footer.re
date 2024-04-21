let footerCss = [%cx
  {|
display: flex;
gap: 2rem;
flex-wrap: wrap;

padding-top: 2rem;
padding-bottom: 2rem;
border-top: 1px solid $(Theme.backgroundSubtle);

color: $(Theme.foregroundSubtle);
font-family: "Poppins";
|}
];

let infoSectionCss = [%cx
  {|
flex-grow: 1;

& > h1 {
  font-size: 1.5rem;
  margin-top: 0;
  margin-bottom: 0.5rem;
}
& > p {
  margin-top: 0.5rem;
  margin-bottom: 0;
}

$(Theme.afterSmall) {
  flex-grow: 0;
}
|}
];

let linkSectionCss = [%cx
  {|
& > h1 {
  font-size: 1.5rem;
  margin-top: 0;
  margin-bottom: 0.5rem;
}
& > a {
  display: block;
  margin-top: 0.5rem;
  margin-bottom: 0;
}
|}
];

let footerLinkCss = [%cx {|

|}];

[@react.component]
let make = () => {
  <footer className=footerCss>
    <article className=infoSectionCss>
      <h1> {React.string("sabihin.ph")} </h1>
      <p> {React.string("anonymous and secure confessions")} </p>
      <p> {React.string("built with <3 in OCaml+Reason")} </p>
      <p> {React.string("(c) Justin Garcia, 2024")} </p>
    </article>
    <nav className=linkSectionCss role="navigation">
      <h1> {React.string("links")} </h1>
      <a href="https://github.com/purefunctor/sabihin">
        {React.string("GitHub Repository")}
      </a>
      <a href="https://github.com/sponsors/purefunctor">
        {React.string("GitHub Sponsors")}
      </a>
    </nav>
  </footer>;
};
