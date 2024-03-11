module Book = {
  [@react.component]
  let make = (~size=?) => {
    let size = Option.value(size, ~default="1rem");
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M3 18.5V5a3 3 0 0 1 3-3h14a1 1 0 0 1 1 1v18a1 1 0 0 1-1 1H6.5A3.5 3.5 0 0 1 3 18.5M19 20v-3H6.5a1.5 1.5 0 0 0 0 3zM5 15.337A3.486 3.486 0 0 1 6.5 15H19V4H6a1 1 0 0 0-1 1z"
      />
    </svg>;
  };
};

module Chat = {
  [@react.component]
  let make = (~size=?) => {
    let size = Option.value(size, ~default="1rem");
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M2 8.994A5.99 5.99 0 0 1 8 3h8c3.313 0 6 2.695 6 5.994V21H8c-3.313 0-6-2.695-6-5.994zM14 11v2h2v-2zm-6 0v2h2v-2z"
      />
    </svg>;
  };
};

module Sparkle = {
  [@react.component]
  let make = (~size=?) => {
    let size = Option.value(size, ~default="1rem");
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M14 4.438A2.437 2.437 0 0 0 16.438 2h1.125A2.437 2.437 0 0 0 20 4.438v1.125A2.437 2.437 0 0 0 17.563 8h-1.125A2.437 2.437 0 0 0 14 5.563zM1 11a6 6 0 0 0 6-6h2a6 6 0 0 0 6 6v2a6 6 0 0 0-6 6H7a6 6 0 0 0-6-6zm3.876 1A8.038 8.038 0 0 1 8 15.124A8.038 8.038 0 0 1 11.124 12A8.038 8.038 0 0 1 8 8.876A8.038 8.038 0 0 1 4.876 12m12.374 2A3.25 3.25 0 0 1 14 17.25v1.5A3.25 3.25 0 0 1 17.25 22h1.5A3.25 3.25 0 0 1 22 18.75v-1.5A3.25 3.25 0 0 1 18.75 14z"
      />
    </svg>;
  };
};
