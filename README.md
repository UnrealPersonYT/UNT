# UNT — Universally Not Text

UNT is a compact, self‑synchronizing binary framing encoding built around a single invariant:

## **UNT is not text.**

Every UNT byte is deliberately chosen from the ranges **0x80–0xFF**, ensuring the output is:

* not ASCII
* not UTF‑8
* not printable
* not interpretable as text by any editor or terminal

UNT is a binary envelope, not a character encoding.
It is designed for structure, reversibility, and safety — not readability.

---

## How UNT Works

UNT packs raw data into **6‑bit units**, then wraps each unit in a high‑bit prefix:

* **Continuation bytes**: `11xxxxxx` (0xC0–0xFF)
* **Final byte**: `10xxxxxx` (0x80–0xBF)

The encoder:

1. Accumulates raw bits
2. Emits 6‑bit chunks as high‑bit bytes
3. Marks the last chunk with a `10` prefix

There is:

* no padding
* no alignment requirement
* no ASCII leakage
* no possibility of forming valid UTF‑8

UNT streams are always binary, always structured, and always self‑terminating.

---

## Why UNT Is the Opposite of a Text Encoding

Text encodings are designed to be:

* printable
* readable
* valid character sequences
* compatible with terminals and editors

UNT is designed to be **none of those things**.

* Text encodings try to be readable.
  **UNT tries to be unreadable.**

* Text encodings avoid high‑bit bytes.
  **UNT requires them.**

* Text encodings form valid codepoints.
  **UNT forms bytes invalid in every text encoding.**

* Text encodings collapse structure into characters.
  **UNT exposes structure through prefix bits.**

UNT is a binary framing encoding that *looks like it might be text* but is **provably not text**.

---

## Structural Safety

UNT has built‑in corruption detection:

1. **Any byte < 0x80 is invalid**
   Instantly detects ASCII contamination.

2. **Prefix bits define structure**

   * `11xxxxxx` = continuation
   * `10xxxxxx` = final
     Anything else indicates corruption or truncation.

These rules make UNT:

* self‑terminating
* self‑synchronizing
* easy to validate
* resistant to silent corruption

---

## What UNT Is For

UNT is ideal when you need a **binary envelope** that:

* never collides with text
* never forms valid UTF‑8
* never leaks ASCII
* can be embedded inside other binary encodings
* can be scanned, validated, and resynchronized
* preserves structure under concatenation or slicing

Use UNT when you want an encoding that is:

* binary
* explicit
* reversible
* structurally safe
* and **guaranteed not to be interpreted as text**

UNT is **Universally Not Text**.