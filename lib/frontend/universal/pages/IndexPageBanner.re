let bannerCss = [%cx
  {|
text-align: center;

margin-top: 2rem;
margin-bottom: 2rem;

padding-bottom: 2rem;
border-bottom: 1px solid $(Theme.backgroundSubtle);

$(Theme.afterMedium) {
  text-align: left;
}
|}
];

let titleCss = [%cx
  {|
font-size: 1.75rem;
margin-top: 0;
margin-bottom: 0.5rem;

$(Theme.afterMedium) {
  font-size: 4rem;
}
|}
];

let subtitleCss = [%cx
  {|
margin-top: 0.5rem;
margin-bottom: 0;

$(Theme.afterMedium) {
  font-size: 1.5rem;
}
|}
];

let titleGradientCss = [%cx
  {|
background:
  linear-gradient(
    to right,
    var(--primary),
    var(--accent),
    var(--secondary));
background-clip: text;
color: rgba(0, 0, 0, 0);
|}
];

let actionButtonsCss = [%cx
  {|
margin-top: 2rem;
display: flex;
gap: 1rem;
justify-content: center;

$(Theme.afterMedium) {
  justify-content: initial;
}
|}
];

let iconTextCss = [%cx {|
display: flex;
gap: 4px;
|}];

[@react.component]
let make = () => {
  <section className=bannerCss>
    <header>
      <h1 className=titleCss>
        <span className=titleGradientCss>
          {React.string("End-to-End Encrypted")}
        </span>
        <br />
        <span className=titleGradientCss>
          {React.string("Anonymous Confessions")}
        </span>
      </h1>
      <p role="doc-subtitle" className=subtitleCss>
        {React.string(
           "sabihin.ph is a privacy-first platform for anonymous confessions.",
         )}
      </p>
      <nav className=actionButtonsCss>
        <Link className=Theme.primaryButton href="/register">
          <span className=iconTextCss>
            <Icons.SparkleLine size="1rem" />
            {React.string("Get Started")}
          </span>
        </Link>
        <Link className=Theme.ghostButton href="/learn">
          <span className=iconTextCss>
            <Icons.BookLine size="1rem" />
            {React.string("Learn More")}
          </span>
        </Link>
      </nav>
    </header>
  </section>;
};