let mediumGroupCss = [%cx
  {|
  display: none;
  $(Theme.afterMedium) {
    display: flex;
  }
  gap: 2rem;
|}
];

let linkCss = [%cx
  {|
  display: flex;
  gap: 0.5rem;
  align-items: center;
  &:hover {
    color: $(Theme.secondary);
  }
  transition: color 150ms ease-out;
  font-size: 1.2rem;
|}
];

let profileButtonCss = [%cx
  {|
  height: 2.5rem;
  width: 2.5rem;
  background-color: $(Theme.primary);
  border: none;
  padding: 0;
  border-radius: 100%;
  cursor: pointer;
|}
];

[@react.component]
let make = () => {
  <div className=mediumGroupCss>
    <Link href="/learn" className=linkCss>
      <Icons.QuestionLink />
      {React.string("Learn")}
    </Link>
    <Link href="/inbox" className=linkCss>
      <Icons.InboxLine />
      {React.string("Inbox")}
    </Link>
    <Link href="/write" className=linkCss>
      <Icons.QuillLine />
      {React.string("Write")}
    </Link>
    <Link href="/profile" className=profileButtonCss />
  </div>;
};
