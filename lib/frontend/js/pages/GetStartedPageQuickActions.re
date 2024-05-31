let articleCss = [%cx
  {|
& > h1 {
  font-size: 2rem;
  margin-top: 0;
  margin-bottom: 0;
}
& > p {
  margin-bottom: 0;
}
|}
];

let navCss = [%cx
  {|
display: grid;
grid-template-columns: repeat(2, minmax(0, 1fr));
grid-template-columns: repeat(2, minmax(0, 1fr));
gap: 1rem;
margin-top: 1rem;
|}
];

let itemCss = [%cx
  {|
height: 12rem;
background-color: $(Theme.background11);
border-color: $(Theme.background9);
border-radius: 8px;
border-style: solid;
border-width: 2px;

display: flex;
flex-direction: column;
align-items: center;
justify-content: center;
gap: 0.5rem;

transition:
  border-color 150ms ease-out;

& > svg {
  color: $(Theme.primary);
}

&:hover {
  border-color: $(Theme.primary);
}
|}
];

[@react.component]
let make = () => {
  <>
    <article className=articleCss>
      <h1> {React.string("Awesome!")} </h1>
      <p> {React.string("Here's some links to get started with:")} </p>
    </article>
    <nav className=navCss>
      <Link href="/inbox" className=itemCss>
        <Icons.InboxLine size="3rem" />
        {React.string("Inbox")}
      </Link>
      <Link href="/compose" className=itemCss>
        <Icons.QuillLine size="3rem" />
        {React.string("Compose")}
      </Link>
      <Link href="/profile" className=itemCss>
        <Icons.LinkM size="3rem" />
        {React.string("Share")}
      </Link>
      <Link href="/learn" className=itemCss>
        <Icons.QuestionLink size="3rem" />
        {React.string("Learn")}
      </Link>
    </nav>
  </>;
};
